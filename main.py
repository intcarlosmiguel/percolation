import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

# Exemplo de matriz (você pode substituir por qualquer outra matriz)
matrix = np.loadtxt('test.txt')
matrix = matrix.reshape((int(np.sqrt(len(matrix))), int(np.sqrt(len(matrix)))))
# Configuração do heatmap
plt.figure(figsize=(6, 6))
sns.heatmap(matrix, cmap="coolwarm", cbar=False, square=True)
plt.axis('off')

# Exibindo o gráfico
plt.show()