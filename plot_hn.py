import csv
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
import sys

print(sys.argv) # ['plot_filtered_tones_self_contained.py', 'tones3.csv', '128']

if len(sys.argv) < 2:
    print(f"Usage: {sys.argv[0]} <tones_csv> [filtered_csv]")
    sys.exit(1)

csv_file = sys.argv[1]

n = []
amplitude = []

with open( csv_file, newline="" ) as f:
    reader = csv.DictReader(f, skipinitialspace=True)

    for row in reader:
        n.append(float(row["k"]))
        amplitude.append(float(row["h_real"]))

fig, ax = plt.subplots()
ax.plot(n, amplitude, marker="o")
ax.xaxis.set_major_locator(MultipleLocator(10))
ax.xaxis.set_minor_locator(MultipleLocator(5))
plt.xlabel("n")
plt.ylabel("Amplitude")
plt.title("Filtered tritone")
plt.grid()
plt.show()
