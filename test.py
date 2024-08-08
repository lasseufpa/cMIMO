import itertools
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import plotly.graph_objects as go


data = pd.read_csv('output.csv', header=None)
data.columns = ['Nr', 'Nt', 'BER','SNR', 'cap']
# Filter data for NoiseLevel=3
# data = data[data['NoiseLevel'] == 3]

fig, ax1 = plt.subplots()

# Define a list of markers
markers = itertools.cycle(('p', 'd', 's', 'o', '+', 'v', 'x', 'h', '<', '>'))

# Group data by 'Nr' and 'Nt'
grouped = data.groupby(['Nr', 'Nt'])

# Plot BER
line1, = ax1.semilogy(data['SNR'], data['BER'], label='BER', color='purple')
marker_handles = []  # List to store marker handles for the legend
for (nr, nt), group in grouped:
    marker = ax1.scatter(group['SNR'], group['BER'], label=f'Nr={nr} Nt={nt}', color='purple', marker=next(markers), s=60)  # Increase marker size
    marker_handles.append(marker)
ax1.set_ylabel('BER', fontsize=14)
ax1.set_ylim([10**(-5), 1])
ax1.grid(True, which ="both")
ax1.tick_params(axis='both', which='major', labelsize=10.5)

# Create a second y-axis
ax2 = ax1.twinx()

# Reset markers
markers = itertools.cycle(('p', 'd', 's', 'o', '+', 'v', 'x', 'h', '<', '>'))

# Plot Capacity
line2, = ax2.plot(data['SNR'], data['cap'], label='Capacity', color='green')
for (nr, nt), group in grouped:
    marker = ax2.scatter(group['SNR'], group['cap'], label=f'Nr={nr} Nt={nt}', color='green', marker=next(markers), s=60)  # Increase marker size
ax2.set_ylabel('Capacity (bits/symbol/layer)', fontsize=14)
ax2.tick_params(axis='both', which='major', labelsize=10.5)

# Create custom legend
legend1 = plt.legend([line1, line2], ['BER', 'Capacity'], loc='upper center', handlelength=4)  # Increase handlelength
plt.gca().add_artist(legend1)
legend2 = plt.legend(marker_handles, [f'Nr={nr} Nt={nt}' for (nr, nt), group in grouped], loc='center left')
for handle in legend2.legend_handles:  # Change the color of all markers to gray
    handle.set_color('gray')
legend1.set_bbox_to_anchor([0.6, 1])  # Move the legend box a bit to the left
legend2.set_bbox_to_anchor([0.5, 0.25])  # Move the legend box a bit to the left

# Increase linewidth of legend lines
for line in legend1.get_lines():
    line.set_linewidth(3)
ax1.set_xlabel('SNR (dB)', fontsize=14)

# Adjust plot area
plt.subplots_adjust(left=0.1, right=0.9, top=0.9, bottom=0.2)

# Save the figure as a PDF file with tight bounding box
plt.savefig('figure.eps', format='eps', bbox_inches='tight')

plt.show()