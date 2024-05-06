import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns



# Load your data
data = pd.read_csv('output.csv', header=None)
data.columns = ['Test', 'Nr', 'Nt', 'NoiseLevel', 'ErrorPercentage', 'BER']
print(data.describe())

#data = data[(data['Nr'] <= 64) & (data['Nt'] <= 64)]
#data = data[(data['Nr'] >= 2) & (data['Nr'] <= 60)]
#data = data.head(16) # Considerar apenas os 24 primeiros registros

noise_intervals = {
    0: '[-0.001, 0.001]',
    1: '[-0.01, 0.01]',
    2: '[-0.5, 0.5]',
    3: '[-1, 1]'
}

data['NoiseLevel'] = data['NoiseLevel'].map(noise_intervals)

# Group the data by Nt and NoiseLevel and calculate the mean of ErrorPercentage
grouped = data.groupby(['Nt', 'NoiseLevel'])['BER'].mean().reset_index()


# Create a line plot for each noi# Create a line plot for each noise level
sns.set_style("whitegrid")
plt.figure(figsize=(10, 6))
sns.lineplot(x='Nt', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", linewidth=2.5)

# Add points without legend
sns.scatterplot(x='Nt', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", s=100, legend=False)

plt.xlabel('Antennas Nt', fontsize=13)
plt.ylabel('BER', fontsize=13)
plt.title('BER by number of antennas and noise level', fontsize=16)
plt.legend(title='Noise level intervals')
plt.show()

#data = data.head(16) # Considerar apenas os 24 primeiros registros
# Repeat for the second plot
grouped = data.groupby(['Nr', 'NoiseLevel'])['BER'].mean().reset_index()

plt.figure(figsize=(10, 6))
sns.lineplot(x='Nr', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", linewidth=2.5)

# Add points without legend
sns.scatterplot(x='Nr', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", s=100, legend=False)

plt.xlabel('Antennas Nr', fontsize=13)
plt.ylabel('BER', fontsize=13)
plt.title('BER by number of antennas and noise level', fontsize=16)
plt.legend(title='Noise level intervals')
plt.show()

