import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np


## Load your data
#data = pd.read_csv('output.csv', header=None)
#data.columns = ['Test', 'Nr', 'Nt', 'NoiseLevel', 'ErrorPercentage', 'BER','SNR','EVM', 'cap']
#print(data.describe())
#
##data = data[(data['Nr'] <= 64) & (data['Nt'] <= 64)]
##data = data[(data['Nr'] >= 2) & (data['Nr'] <= 60)]
##data = data.head(16) # Considerar apenas os 24 primeiros registros
#
#noise_intervals = {
#    3: '[-1, 1]'
#}
#
#data['NoiseLevel'] = data['NoiseLevel'].map(noise_intervals)
#
## Group the data by Nt and NoiseLevel and calculate the mean of ErrorPercentage
## Group the data by SNR and NoiseLevel and calculate the mean of BER
#grouped = data.groupby(['SNR', 'NoiseLevel'])['BER'].mean().reset_index()
#
## Create a line plot for each noise level
#sns.set_style("whitegrid")
#plt.figure(figsize=(10, 6))
#sns.lineplot(x='SNR', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", linewidth=2.5)
#
## Add points without legend
#sns.scatterplot(x='SNR', y='BER', hue='NoiseLevel', data=grouped, palette="tab10", s=100, legend=False)
#
#plt.xlabel('SNR', fontsize=13)
#plt.ylabel('BER', fontsize=13)
#plt.title('BER by SNR', fontsize=16)
#plt.show()
#
##import pandas as pd
##import plotly.graph_objects as go
##
### Load your data
##data = pd.read_csv('output.csv', header=None)
##data.columns = ['Test', 'Nr', 'Nt', 'NoiseLevel', 'ErrorPercentage', 'BER','SNR','EVM', 'cap']
##
##noise_intervals = {
##    3: '[-1, 1]'
#}
#
#data['NoiseLevel'] = data['NoiseLevel'].map(noise_intervals)
#
## Group the data by SNR and NoiseLevel and calculate the mean of BER
#grouped_ber = data.groupby(['SNR', 'NoiseLevel'])['BER'].mean().reset_index()
#
## Create a line plot for each noise level
#fig_ber = go.Figure()
#
#for noise_level in grouped_ber['NoiseLevel'].unique():
#    subset = grouped_ber[grouped_ber['NoiseLevel'] == noise_level]
#    fig_ber.add_trace(go.Scatter(x=subset['SNR'], y=subset['BER'], mode='lines+markers', name=noise_level, showlegend=False))
#
#fig_ber.update_layout(
#    title='BER by SNR',
#    xaxis_title='SNR',
#    yaxis_title='BER'
#)
#
#fig_ber.show()
#
## Group the data by SNR and NoiseLevel and calculate the mean of cap
#grouped_cap = data.groupby(['SNR', 'NoiseLevel'])['cap'].mean().reset_index()
#
## Create a line plot for each noise level
#fig_cap = go.Figure()
#
#for noise_level in grouped_cap['NoiseLevel'].unique():
#    subset = grouped_cap[grouped_cap['NoiseLevel'] == noise_level]
#    fig_cap.add_trace(go.Scatter(x=subset['SNR'], y=subset['cap'], mode='lines+markers', name=noise_level, showlegend=False))
#
#fig_cap.update_layout(
#    title='cap by SNR',
#    xaxis_title='SNR',
#    yaxis_title='cap'
#)
#
#fig_cap.show()

import pandas as pd
import matplotlib.pyplot as plt

# Load your data
data = pd.read_csv('output.csv', header=None)

# Select every fourth row starting from 0
#data = data.iloc[::4, :]

data.columns = ['Test', 'Nr', 'Nt', 'NoiseLevel', 'ErrorPercentage', 'BER','SNR','EVM', 'cap']

noise_intervals = {
    3: '',
    }

data['NoiseLevel'] = data['NoiseLevel'].map(noise_intervals)

# Group the data by SNR and NoiseLevel and calculate the mean of BER
grouped_ber = data.groupby(['SNR', 'NoiseLevel'])['BER'].mean().reset_index()

# Create a semilogy plot for each noise level
for noise_level in grouped_ber['NoiseLevel'].unique():
    subset = grouped_ber[grouped_ber['NoiseLevel'] == noise_level]
    plt.semilogy(subset['SNR'], subset['BER'], label=noise_level) 
                 #marker='s')

plt.ylabel('BER')
plt.xlabel('SNR (dB)')
plt.ylim([10**(-5), 1])
plt.grid(True, which ="both")
plt.legend()
plt.show()

# Group the data by SNR and NoiseLevel and calculate the mean of cap
grouped_cap = data.groupby(['SNR', 'NoiseLevel'])['cap'].mean().reset_index()

# Create a plot for each noise level
plt.figure()
for noise_level in grouped_cap['NoiseLevel'].unique():
    subset = grouped_cap[grouped_cap['NoiseLevel'] == noise_level]
    plt.plot(subset['SNR'], subset['cap'], label=noise_level, marker='o')

plt.ylabel('Capacity (bits/symbol)')
plt.xlabel('SNR (dB)')
plt.grid(True, which ="both")
plt.legend()
plt.show()

