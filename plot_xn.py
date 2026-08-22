import csv
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

index = []
amplitude = []

with open("signal.csv", newline="") as f:
    reader = csv.DictReader(f, skipinitialspace=True)

    for row in reader:
        index.append(float(row["index"]))
        amplitude.append(float(row["x[n]"]))

fig, ax = plt.subplots()
ax.plot(index, amplitude, marker="o")
ax.xaxis.set_major_locator(MultipleLocator(1))
plt.xlabel("n")
plt.ylabel("Amplitude")
plt.title("A 10 kHz signal sampled at 100 kHz")
plt.grid()
plt.show()
