import matplotlib.pyplot as plt
import numpy as np

data = """
KR KMP bndmq2 Boyer-Moore BXS Backward-OM skip Hash3 FS SSM SBNDM BSDM
2 4 8 16 32 64 128 256
2 SBNDM bndmq2 bndmq2 BSDM BSDM BSDM BSDM BSDM
4 KR bndmq2 bndmq2 bndmq2 bndmq2 BSDM BSDM BXS
8 Hash3 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 BSDM BSDM
16 SBNDM bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 BSDM
32 SBNDM bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2
64 SBNDM bndmq2 BXS bndmq2 bndmq2 bndmq2 bndmq2 BXS
128 SBNDM BXS Hash3 bndmq2 BXS BXS BXS BXS
256 Backward-OM Hash3 Hash3 Hash3 Hash3 BXS BXS BXS
512 Backward-OM Hash3 Hash3 Hash3 Hash3 BXS BXS BXS
1024 SBNDM bndmq2 Hash3 Hash3 Hash3 bndmq2 Hash3 BXS
2048 SBNDM bndmq2 Hash3 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2
4096 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2 bndmq2
"""

# Parse the data
lines = data.strip().split('\n')
algorithms = lines[0].split()
sigmas = list(map(int, lines[1].split()))
patterns_data = [line.split() for line in lines[2:]]

pattern_sizes = [int(row[0]) for row in patterns_data]
pattern_algorithms = [row[1:] for row in patterns_data]

# Create a color map for algorithms
unique_algorithms = sorted(set(alg for row in pattern_algorithms for alg in row))
colors = plt.cm.tab20(np.linspace(0, 1, len(unique_algorithms)))
alg_color_map = {alg: colors[i] for i, alg in enumerate(unique_algorithms)}

# Create a grid for the heatmap
heatmap = np.zeros((len(pattern_sizes), len(sigmas), 3))

for i, row in enumerate(pattern_algorithms):
    for j, alg in enumerate(row):
        heatmap[i, j] = alg_color_map[alg][:3]

# Plot the heatmap
fig, ax = plt.subplots(figsize=(14, 8))

ax.imshow(heatmap, aspect='auto')

# Set axis labels
ax.set_xticks(np.arange(len(sigmas)))
ax.set_xticklabels(sigmas)
ax.set_xlabel('Sigma (Alphabet size)')

ax.set_yticks(np.arange(len(pattern_sizes)))
ax.set_yticklabels(pattern_sizes)
ax.set_ylabel('Pattern size (m)')

# Add a legend
legend_elements = [plt.Line2D([0], [0], marker='o', color='w', markerfacecolor=alg_color_map[alg], markersize=10, label=alg)
                   for alg in unique_algorithms]
ax.legend(handles=legend_elements, loc='upper right', bbox_to_anchor=(1.15, 1))

plt.title('Optimal Algorithms for Various Sigma and Pattern Sizes')
plt.show()
