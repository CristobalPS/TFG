import pandas as pd
from scipy.stats import shapiro
import sys

# Leer los datos desde el archivo CSV
df = pd.read_csv(sys.argv[1])

# Extraer los datos de cada grupo
group1 = df['Fitness 1'].dropna().tolist()
group1 = df['Fitness 2'].dropna().tolist()
group1 = df['Fitness 3'].dropna().tolist()
group1 = df['Fitness 4'].dropna().tolist()


# Realizar el test de Shapiro-Wilk para cada grupo
for column in df.columns:
    stat, p_value = shapiro(df[column])
    print(f"Grupo {column}:")
    print(f"Estadística W: {stat}")
    print(f"Valor p: {p_value}\n")
