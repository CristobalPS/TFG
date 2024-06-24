import pandas as pd
from scipy.stats import kruskal
import sys

# Leer los datos desde el archivo CSV
df = pd.read_csv(sys.argv[1])

# Extraer los datos de cada grupo
group1 = df['Fitness CUDA'].dropna().tolist()
group2 = df['Fitness OpenMP'].dropna().tolist()
group3 = df['Fitness Python'].dropna().tolist()

# Realizar el test de Kruskal-Wallis
stat, p_value = kruskal(group1, group2, group3)

print(f'Estadístico de Kruskal-Wallis: {stat}')
print(f'Valor p: {p_value}')

# Interpretar el resultado
alpha = 0.05
if p_value < alpha:
    print('Hay diferencias significativas entre los grupos.')
else:
    print('No hay diferencias significativas entre los grupos.')

