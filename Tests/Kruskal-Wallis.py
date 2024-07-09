import pandas as pd
from scipy.stats import kruskal
import sys
from scikit_posthocs import posthoc_dunn

# Leer los datos desde el archivo CSV
df = pd.read_csv(sys.argv[1])

# Definir el número de filas por grupo
rows_per_group = 10

# Inicializar una lista para almacenar los resultados
results = []

# Obtener el número de columnas
num_columns = df.shape[1]

# Iterar sobre cada subconjunto de filas
for i in range(0, len(df), rows_per_group):
    subset = df.iloc[i:i + rows_per_group]
    
    # Iterar sobre cada conjunto de 3 columnas
    for start_col in range(0, num_columns, 3):
        end_col = start_col + 3
        if end_col <= num_columns:
            col_set = df.columns[start_col:end_col]
            groups = [subset[col].dropna().tolist() for col in col_set]
            
            # Verificar si hay suficientes datos en todos los grupos
            if all(len(group) > 0 for group in groups):
                stat, p_value = kruskal(*groups)
                results.append((i // rows_per_group + 1, col_set.tolist(), stat, p_value))
            else:
                results.append((i // rows_per_group + 1, col_set.tolist(), None, None))

# Mostrar los resultados
for result in results:
    group_num, col_set, stat, p_value = result
    print(f'Grupo {group_num}, Conjunto de columnas {col_set}:')
    if stat is not None and p_value is not None:
        print(f'Estadístico de Kruskal-Wallis: {stat}')
        print(f'Valor p: {p_value}')
        
        # Interpretar el resultado
        alpha = 0.05
        if p_value < alpha:
            print('Hay diferencias significativas entre los grupos.')
        else:
            print('No hay diferencias significativas entre los grupos.')
    else:
        print('No hay suficientes datos para realizar el test.')
    print()



