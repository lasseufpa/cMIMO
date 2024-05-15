import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np


# Load your data
data = pd.read_csv('output.csv', header=None)
data.columns = ['Test', 'Nr', 'Nt', 'NoiseLevel', 'ErrorPercentage', 'BER','SNR','EVM']
print(data.describe())

#data = data[(data['Nr'] <= 64) & (data['Nt'] <= 64)]
#data = data[(data['Nr'] >= 2) & (data['Nr'] <= 60)]
#data = data.head(16) # Considerar apenas os 24 primeiros registros

noise_intervals = {
    #2: '[0,5, 0,5]',
    3: '[-1, 1]'
}

data['NoiseLevel'] = data['NoiseLevel'].map(noise_intervals)

# Group the data by Nt and NoiseLevel and calculate the mean of ErrorPercentage
grouped = data.groupby(['SNR', 'NoiseLevel'])['BER'].mean().reset_index()
#grouped['BER'] = -np.log10(grouped['BER'])

# Create a line plot for each noi# Create a line plot for each noise level
sns.set_style("whitegrid")
plt.figure(figsize=(10, 6))
sns.lineplot(x='SNR', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", linewidth=2.5)

# Add points without legend
sns.scatterplot(x='SNR', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", s=100, legend=False)

plt.xlabel('SNR', fontsize=13)
plt.ylabel('BER', fontsize=13)
plt.title('BER by SNR', fontsize=16)
plt.legend(title='Noise level intervals')
plt.show()

#data = data.head(16) # Considerar apenas os 24 primeiros registros
# Repeat for the second plot
grouped = data.groupby(['SNR', 'NoiseLevel'])['BER'].mean().reset_index()
#grouped['BER'] = np.log10(grouped['BER'])


plt.figure(figsize=(10, 6))
sns.lineplot(x='SNR', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", linewidth=2.5)

# Add points without legend
sns.scatterplot(x='SNR', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", s=100, legend=False)

plt.xlabel('SNR', fontsize=13)
plt.ylabel('BER', fontsize=13)
plt.title('BER by SNR', fontsize=16)
plt.legend(title='Noise level intervals')
plt.show()

sns.set_style("whitegrid")
plt.figure(figsize=(10, 6))

# Get unique combinations of Nr and Nt
combinations = data[['Nr', 'Nt']].drop_duplicates().values

# For each combination of Nr and Nt, plot BER vs SNR
for nr, nt in combinations:
    subset = data[(data['Nr'] == nr) & (data['Nt'] == nt)]
    sns.lineplot(x='SNR', y='BER', data=subset, label=f'Nr={nr}, Nt={nt}')

plt.xlabel('SNR (dB)', fontsize=13)
plt.ylabel('BER', fontsize=13)
plt.title('BER by number of antennas and SNR', fontsize=16)
plt.legend(title='Antenna configurations')
plt.show()
plt.hist(data['BER'], bins=10)
plt.show()

