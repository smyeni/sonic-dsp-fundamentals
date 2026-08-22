import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator

df = pd.read_csv("signal.csv", skipinitialspace=True)

plt.figure(figsize=(10, 4))
plt.plot(df["index"], df["x[n]"], marker='o', markersize=3)

ax = plt.gca()
ax.xaxis.set_major_locator(MultipleLocator(1))

plt.xlabel("n")
plt.ylabel("Amplitude")
plt.title("10kHz sine, sampled at 100kHz")
plt.grid(True)
plt.tight_layout()
plt.savefig("signal.png", dpi=150)
plt.show()
