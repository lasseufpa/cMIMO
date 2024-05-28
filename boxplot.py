import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Ler o arquivo CSV:
df = pd.read_csv('/home/marco/Documentos/c_mimo/lambsa.csv', header=None)
df = df.rename(columns={0: 'Nr', 1: 'Nt'})
for column in df.columns[2:]:
    df = df.rename(columns={column: str(column - 1)})
df = df.T
df = df.iloc[:, ::-1]

# Gerar o boxplot:
sns.boxplot(data=df[2:], palette='Purples', order=df[2:].columns)
plt.grid()
plt.title('Boxplot of singular values of channels (λ)')
plt.ylabel('Singular values')
plt.xlabel('Number of Receiving and Transmitting Antennas (Nr, Nt)')
labels = ['(2,4)', '(4,8)', '(8,16)', '(16,32)', '(32,64)', '(64,128)', '(128,256)']
plt.xticks(ticks=range(len(df[2:].columns)), labels=labels)
plt.savefig('boxplot.eps', format='eps', bbox_inches='tight')
plt.show()
