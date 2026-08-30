import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
from datetime import datetime as dt

#Pandas file read
df = pd.read_csv("signal.csv", skipinitialspace=True)

fig, (ax_sig, ax_hann, ax_win, ax_bins, ax_hz) = plt.subplots(5, 1, figsize=(14, 8))
#fig, (ax_sig, ax_hann, ax_win, ax_bins, ax_hz) = plt.subplots(5, 1)
ax_sig.plot(df["index"], df["x[n]"], marker='o', markersize=3)

ax_sig.xaxis.set_major_locator(MultipleLocator(10))
ax_sig.xaxis.set_minor_locator(MultipleLocator(1))

ax_sig.set_xlabel("n")
ax_sig.set_ylabel("Amplitude")
ax_sig.set_title("10kHz sine, sampled at 100kHz")
ax_sig.grid(True)

##################################################

ax_hann.plot(df["index"], df["hann"], marker='o', markersize=3)

ax_hann.xaxis.set_major_locator(MultipleLocator(10))
ax_hann.xaxis.set_minor_locator(MultipleLocator(1))

ax_hann.set_xlabel("[Hz]")
ax_hann.set_ylabel("Amplitude")
ax_hann.set_title("Hanning window")
ax_hann.grid(True)

##################################################

ax_win.plot(df["index"], df["x_win"], marker='o', markersize=3)

ax_win.xaxis.set_major_locator(MultipleLocator(10))
ax_win.xaxis.set_minor_locator(MultipleLocator(1))

ax_win.set_xlabel("[Hz]")
ax_win.set_ylabel("Amplitude")
ax_win.set_title("Windowed sinusoid [Hanning window]")
ax_win.grid(True)

#################################################
df2 = pd.read_csv("spectrum.csv", skipinitialspace=True)
 
# --- left panel: magnitude vs bin index (k) ---
ax_bins.plot(df2["n"], 20 * np.log10(df2["magn_raw"]), marker='o', markersize=3)
ax_bins.set_xlabel("Bin index (k)")
ax_bins.set_ylabel("Amplitude")
ax_bins.set_title("Spectrum by bin")
ax_bins.grid(True)
ax_bins.xaxis.set_major_locator(MultipleLocator(10))
ax_bins.xaxis.set_minor_locator(MultipleLocator(1))
 
# --- right panel: magnitude vs frequency (Hz) ---
ax_hz.plot(df2["freq"], 20 * np.log10(df2["magn_hann"]), marker='o', markersize=3, color='tab:orange')
ax_hz.set_xlabel("Frequency [Hz]")
ax_hz.set_ylabel("Amplitude")
ax_hz.set_title("Spectrum by frequency")
ax_hz.grid(True)
ax_hz.xaxis.set_major_locator(MultipleLocator(5000))
ax_hz.xaxis.set_minor_locator(MultipleLocator(1000))

#####################################################

plt.tight_layout()
fname = "signal_{:%Y%m%d_%H%M%S}.png".format(dt.now())
plt.savefig( fname, dpi=150 )
plt.show()
