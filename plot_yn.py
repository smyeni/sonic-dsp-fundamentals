import csv
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

n = []
amplitude = []

with open("filtered_tones3.csv", newline="") as f:
    reader = csv.DictReader(f, skipinitialspace=True)

    for row in reader:
        n.append(float(row["n"]))
        amplitude.append(float(row["y_Real"]))

fig, ax = plt.subplots()
ax.plot(n, amplitude, marker="o")
ax.xaxis.set_major_locator(MultipleLocator(4))
plt.xlabel("n")
plt.ylabel("Amplitude")
plt.title("Filtered tritone")
plt.grid()
plt.show()
