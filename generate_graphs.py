import pandas as pd
import sys
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <csv_file_path>")
    sys.exit(1)

csv_file_path = sys.argv[1]

df_avl = pd.read_csv(csv_file_path)

name = csv_file_path.split('.')[0]

plt.figure(figsize=(10, 6))
plt.plot(df_avl['Algoritmo'], df_avl['miss_rate_FIFO'] * 100, marker='o', linestyle='-', label='Miss Rate')
plt.xlabel('Tamanho da Entrada')
plt.ylabel('Miss Rate (%)')
plt.title('Tamanho da Entrada x Miss Rate (FIFO)')
plt.legend()
plt.grid(True)
plt.savefig(f'{name}_entrada_vs_miss_rate_fifo.png')

plt.figure(figsize=(10, 6))
plt.plot(df_avl['Algoritmo'], df_avl['miss_rate_OPT'] * 100, marker='o', linestyle='-', label='Miss Rate')
plt.xlabel('Tamanho da Entrada')
plt.ylabel('Miss Rate (%)')
plt.title('Tamanho da Entrada x Miss Rate (OPT)')
plt.legend()
plt.grid(True)
plt.savefig(f'{name}_entrada_vs_miss_rate_opt.png')

plt.figure(figsize=(10, 6))
plt.plot(df_avl['Algoritmo'], df_avl['miss_rate_FIFO'] * 100, marker='o', linestyle='-', label='Miss Rate (FIFO)')
plt.plot(df_avl['Algoritmo'], df_avl['miss_rate_OPT'] * 100, marker='o', linestyle='-', label='Miss Rate (OPT)')
plt.xlabel('Tamanho da Entrada')
plt.ylabel('Miss Rate (%)')
plt.title('Tamanho da Entrada x Miss Rate (FIFO vs OPT)')
plt.legend()
plt.grid(True)
plt.savefig(f'{name}_entrada_vs_miss_rate_fifo_vs_opt.png')
