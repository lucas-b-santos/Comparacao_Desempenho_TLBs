import pandas as pd
import sys

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} <csv_file_path>")
    sys.exit(1)

csv_file_path = sys.argv[1]

df = pd.read_csv(csv_file_path)

name = csv_file_path.split('.')[0]

df['miss_rate_FIFO'] = df['miss_rate_FIFO'] * 100
df['miss_rate_OPT'] = df['miss_rate_OPT'] * 100

# Função para converter DataFrame para tabela LaTeX formatada
def df_to_latex(df: pd.DataFrame, caption, label):
    return df.to_latex(
        index=False,
        caption=caption,
        label=label,
        escape=False,
        column_format=" || ".join("c" * len(df.columns)),
        bold_rows=False
    )
    
# Conversão
latex = df_to_latex(df, "Resultados da Simulação de Páginas", f"tab:resultados_simulacao_{name}")

print(latex)