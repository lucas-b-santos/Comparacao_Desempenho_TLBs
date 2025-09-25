#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define PAGE_SIZE 4096 // 4 KB
#define OFFSET_BITS 12 // 2^12 = 4096

// Estrutura para a reference string
typedef struct
{
    unsigned long *pages;
    long size;
} ReferenceString;

unsigned long hex_to_ulong(const char *s)
{
    unsigned long val = 0;
    if (strstr(s, "0x") == s || strstr(s, "0X") == s)
    {
        sscanf(s, "%lx", &val);
    }
    else
    {
        // tenta hex sem 0x, se falhar, decimal
        if (sscanf(s, "%lx", &val) == 0)
        {
            sscanf(s, "%lu", &val);
        }
    }
    return val;
}

ReferenceString carregar_reference_de_trace(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        perror("Erro ao abrir trace");
        exit(1);
    }
    ReferenceString ref;
    ref.pages = malloc(sizeof(unsigned long) * 5000000); // alocação inicial (pode ajustar...)
    ref.size = 0;

    char line[256];
    unsigned long anterior = -1;

    while (fgets(line, sizeof(line), f))
    {
        if (!(line[0] == 'I' || line[0] == 'L' || line[0] == 'S' || line[0] == 'M'))
            continue;
        char addr_str[64];
        if (sscanf(line + 1, "%s", addr_str) != 1)
            continue;

        char *comma = strchr(addr_str, ',');
        if (comma)
            *comma = '\0';

        unsigned long addr = hex_to_ulong(addr_str);
        unsigned long page = addr >> OFFSET_BITS;

        if (page == anterior)
            continue; // ignora acessos repetidos à mesma página

        anterior = page;

        ref.pages[ref.size++] = page;
    }
    fclose(f);
    return ref;
}

ReferenceString carregar_reference_de_arquivo(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        perror("Erro ao abrir reference string");
        exit(1);
    }
    ReferenceString ref;
    ref.pages = malloc(sizeof(unsigned long) * 5000000);
    ref.size = 0;

    char buf[64];
    while (fscanf(f, "%s", buf) == 1)
    {
        unsigned long page = hex_to_ulong(buf);
        ref.pages[ref.size++] = page;
    }
    fclose(f);
    return ref;
}
// Algoritmo FIFO
long simular_FIFO(ReferenceString ref, int frames)
{
    if (frames <= 0)
        return ref.size;
    unsigned long *mem = malloc(sizeof(unsigned long) * frames);
    int filled = 0;
    int pos = 0;
    long faults = 0;

    for (long i = 0; i < ref.size; i++)
    {
        unsigned long p = ref.pages[i];
        int hit = 0;
        for (int j = 0; j < filled; j++)
        {
            if (mem[j] == p)
            {
                hit = 1;
                break;
            }
        }
        if (!hit)
        {
            faults++;
            if (filled < frames)
            {
                mem[filled++] = p;
            }
            else
            {
                mem[pos] = p;
                pos = (pos + 1) % frames;
            }
        }
    }
    free(mem);
    return faults;
}
// Algoritmo ÓTIMO (Belady)
long simular_OPT(ReferenceString ref, int frames)
{
    if (frames <= 0)
        return ref.size;
    unsigned long *mem = malloc(sizeof(unsigned long) * frames);
    int filled = 0;
    long faults = 0;

    for (long i = 0; i < ref.size; i++)
    {
        unsigned long p = ref.pages[i];
        int hit = 0;
        for (int j = 0; j < filled; j++)
        {
            if (mem[j] == p)
            {
                hit = 1;
                break;
            }
        }
        if (!hit)
        {
            faults++;
            if (filled < frames)
            {
                mem[filled++] = p;
            }
            else
            {
                // escolhe a página mais distante no futuro!
                int victim = -1;
                long dist_max = -1;
                for (int j = 0; j < frames; j++)
                {
                    long k;
                    for (k = i + 1; k < ref.size; k++)
                    {
                        if (ref.pages[k] == mem[j])
                            break;
                    }
                    if (k == ref.size)
                    {
                        victim = j;
                        break;
                    }
                    if (k > dist_max)
                    {
                        dist_max = k;
                        victim = j;
                    }
                }
                mem[victim] = p;
            }
        }
    }
    free(mem);
    return faults;
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Uso: %s (--trace arquivo | --ref arquivo) --csv_file <arquivo_csv>\n", argv[0]);
        return 1;
    }

    ReferenceString ref;
    if (strcmp(argv[1], "--trace") == 0)
    {
        ref = carregar_reference_de_trace(argv[2]);
        printf("[info] Trace carregado: %ld acessos\n", ref.size);
    }
    else if (strcmp(argv[1], "--ref") == 0)
    {
        ref = carregar_reference_de_arquivo(argv[2]);
        printf("[info] Reference string carregada: %ld acessos\n", ref.size);
    }
    else
    {
        printf("Parâmetro inválido.\n");
        return 1;
    }

    int frames_list[] = {4, 8, 16, 32, 64, 128};
    int nframes = sizeof(frames_list) / sizeof(frames_list[0]);

    // --- saída CSV ---
    FILE *csv = fopen(argv[4], "w");
    if (!csv)
    {
        perror("Erro ao criar arquivo CSV");
        exit(1);
    }

    // cabeçalho torto...
    fprintf(csv, "Algoritmo");
    fprintf(csv, ",");
    fprintf(csv, "FIFO");
    fprintf(csv, ",");
    fprintf(csv, "miss_rate_FIFO");
    fprintf(csv, ",");
    fprintf(csv, "OPT");
    fprintf(csv, ",");
    fprintf(csv, "miss_rate_OPT");
    fprintf(csv, "\n");

    for (int i = 0; i < nframes; i++)
    {
        fprintf(csv, "%d", frames_list[i]);
        long f_fifo = simular_FIFO(ref, frames_list[i]);
        fprintf(csv, ",%ld", f_fifo);
        fprintf(csv, ",%.6f", (double)f_fifo / ref.size);
        long f_opt = simular_OPT(ref, frames_list[i]);
        fprintf(csv, ",%ld", f_opt);
        fprintf(csv, ",%.6f", (double)f_opt / ref.size);
        fprintf(csv, "\n");
    }

    fclose(csv);
    printf("[info] Resultados salvos em %s\n", argv[4]);

    free(ref.pages);
    return 0;
}