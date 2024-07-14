import pandas as pd
import sys
from scipy.stats import kruskal
from scikit_posthocs import posthoc_dunn
import seaborn as sns
import matplotlib.pyplot as plt

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
                
                print(f'Grupo {i // rows_per_group + 1}, Conjunto de columnas {col_set}:')
                if stat is not None and p_value is not None:
                    print(f'Estadístico de Kruskal-Wallis: {stat}')
                    print(f'Valor p: {p_value}')
                    
                    # Interpretar el resultado
                    alpha = 0.05
                    if p_value < alpha:
                        print('Hay diferencias significativas entre los grupos.')
                        # Realizar test post-hoc de Dunn
                        data = pd.melt(subset[col_set])
                        data.columns = ['Variable', 'Valor']
                        dunn_result = posthoc_dunn(data, val_col='Valor', group_col='Variable', p_adjust='bonferroni')
                        print('Resultados del test post-hoc de Dunn:')
                        print(dunn_result)
                        
                        # Crear un gráfico de calor de los resultados
                        plt.figure(figsize=(10, 8))
                        sns.heatmap(dunn_result, annot=True, fmt=".4f", cmap="coolwarm", annot_kws={"size": 15})
                        plt.title(f'Post-hoc de Dunn para grupo {i // rows_per_group + 1}')
                        plt.show()
                    else:
                        print('No hay diferencias significativas entre los grupos.')
                else:
                    print('No hay suficientes datos para realizar el test.')
                print()
            else:
                results.append((i // rows_per_group + 1, col_set.tolist(), None, None))
