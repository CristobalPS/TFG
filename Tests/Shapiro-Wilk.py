import pandas as pd
from scipy.stats import shapiro
import sys

# Leer los datos desde el archivo CSV
df = pd.read_csv(sys.argv[1])

# Definir el número de filas por grupo
rows_per_group = 10

# Inicializar una lista para almacenar los resultados
results = []

# Iterar sobre cada subconjunto de filas y realizar el test de Shapiro-Wilk
for i in range(0, len(df), rows_per_group):
    subset = df.iloc[i:i + rows_per_group]
    
    group_results = []
    for column in subset.columns:
        group_data = subset[column].dropna().tolist()
        if len(group_data) > 3:  # Shapiro-Wilk test requires at least 3 data points
            stat, p_value = shapiro(group_data)
            group_results.append((column, stat, p_value))
        else:
            group_results.append((column, None, None))
    
    results.append((i // rows_per_group + 1, group_results))

# Mostrar los resultados
for result in results:
    group_num, group_results = result
    print(f'Grupo {group_num}:')
    for column, stat, p_value in group_results:
        if stat is not None and p_value is not None:
            #print(f'Columna {column}:')
            #print(f'Estadística W: {stat}')
            print(f'{p_value}')
        else:
            print(f'Columna {column}: No hay suficientes datos para realizar el test.')
    print()

