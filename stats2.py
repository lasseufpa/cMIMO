import pandas as pd
import plotly.graph_objects as go
import numpy as np

# Load the data
data = pd.read_csv('output.csv')
data.columns = ['Test', 'Nr', 'Nt', 'NoiseLevel', 'ErrorPercentage', 'BER','SNR','EVM', 'cap']

# Select every fourth row, starting from the first
data = data.iloc[::4, :]

# Group the data by the number of antennas and SNR, and calculate the mean BER
grouped = data.groupby(['Nr', 'Nt', 'SNR'])['BER'].mean().reset_index()

# Create a new figure
fig = go.Figure()

# For each group, plot the mean BER vs SNR with markers
for (nr, nt), group in grouped.groupby(['Nr', 'Nt']):
    fig.add_trace(go.Scatter(x=group['SNR'], y=group['BER'], mode='lines+markers', name=f'Nr={nr}, Nt={nt}'))

# Update layout
fig.update_layout(xaxis_title="SNR", yaxis_title="Mean BER", title="Mean BER vs SNR for different antenna configurations", yaxis_type="log")

# Show the plot
fig.show()