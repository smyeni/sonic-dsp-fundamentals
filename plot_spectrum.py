import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
from matplotlib.ticker import MultipleLocator
from datetime import datetime as dt
 
df = pd.read_csv("spectrum.csv", skipinitialspace=True)
 
fig, (ax_bins, ax_hz) = plt.subplots(2, 1, figsize=(14, 5))
 
# --- left panel: magnitude vs bin index (k) ---
ax_bins.plot(df["n"], 20 * np.log10(df["magn_raw"]), marker='o', markersize=3, label='raw')
ax_bins.plot(df["n"], 20 * np.log10(df["magn_hann"]), marker='o', markersize=3, color='tab:orange', label='Hanning')
ax_bins.set_xlabel("Bin index (k)")
ax_bins.set_ylabel("Amplitude")
ax_bins.set_title("Spectrum by bin")
ax_bins.grid(True)
ax_bins.xaxis.set_major_locator(MultipleLocator(10))
ax_bins.xaxis.set_minor_locator(MultipleLocator(1))
#ax_bins.xaxis.set_major_locator(MaxNLocator(20))  # cap tick count, avoid the earlier crash
 
ax_hz.plot(df["freq"], 20 * np.log10(df["magn_hann"]), marker='o', markersize=3, color='tab:orange')
ax_hz.set_xlabel("Frequency [Hz]")
ax_hz.set_ylabel("Amplitude")
ax_hz.set_title("Spectrum by frequency")
ax_hz.grid(True)
ax_hz.xaxis.set_major_locator(MultipleLocator(5000))
ax_hz.xaxis.set_minor_locator(MultipleLocator(1000))
 
fig.suptitle("10kHz sine, sampled at 100kHz")
plt.tight_layout()
fname = "spectrum_{:%Y%m%d_%H%M%S}.png".format(dt.now())
plt.savefig( fname , dpi=150)
#plt.legend()
plt.show()

