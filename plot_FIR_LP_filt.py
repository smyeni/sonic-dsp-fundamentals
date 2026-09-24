import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
from matplotlib.ticker import MultipleLocator
from datetime import datetime as dt
 
df = pd.read_csv("h.csv", skipinitialspace=True)
 
fig, (ax_hamming, ax_h, ax_H) = plt.subplots(3, 1, figsize=(14, 5))

df_hamming = pd.read_csv("h_windowed.csv", skipinitialspace=True)
ax_hamming.plot(
    df_hamming["k"],
    df_hamming["hamming"],
    marker='o',
    markersize=3,
    color='tab:blue',
    label='hamming'
)

ax_y1 = ax_hamming.twinx()
ax_y1.plot(
    df["k"],
    df["h_real"],
    marker='o',
    markersize=3,
    color='tab:red',
    label='not_centered'
)

ax_hamming.set_xlabel("index (n)")
ax_hamming.set_ylabel("Amplitude")
ax_hamming.set_title("Hamming vs uncentered h[n]")
ax_hamming.grid(True)
ax_hamming.xaxis.set_major_locator(MultipleLocator(2))
ax_hamming.xaxis.set_minor_locator(MultipleLocator(1))

# Combine both legends
lines1, labels1 = ax_hamming.get_legend_handles_labels()
lines2, labels2 = ax_y1.get_legend_handles_labels()

ax_hamming.legend(
    lines1 + lines2,
    labels1 + labels2,
    loc='best'
)

#===================================================
 
# --- magnitude vs bin index (k) ---
ax_h.plot(
    df["k"],
    df["h_centered_real"],
    marker='o',
    markersize=3,
    color='tab:blue',
    label='raw'
)

ax_h.set_xlabel("index (n)")
ax_h.set_ylabel("Amplitude")
ax_h.set_title("Filter impulse response")
ax_h.grid(True)
ax_h.xaxis.set_major_locator(MultipleLocator(2))
ax_h.xaxis.set_minor_locator(MultipleLocator(1))

ax_y3 = ax_h.twinx()
ax_y3.plot(
    df["k"],
    df["h_win_real"],
    marker='o',
    markersize=3,
    color='tab:red',
    label='windowed'
)

#===================================================

# Combine both legends
lines1, labels1 = ax_h.get_legend_handles_labels()
lines2, labels2 = ax_y3.get_legend_handles_labels()

ax_h.legend(
    lines1 + lines2,
    labels1 + labels2,
    loc='best'
)

##############################################################################
 
df = pd.read_csv("H.csv", skipinitialspace=True)
ax_H.plot(df["k"], df["H_real"], 
	marker='o', 
	markersize=3, 
	linestyle='--', 
	color='tab:orange',
	label='not_centered')
ax_H.set_xlabel("index [k]")
ax_H.set_ylabel("Amplitude")
ax_H.set_title("Freq response")
ax_H.grid(True)
ax_H.xaxis.set_major_locator(MultipleLocator(2))
ax_H.xaxis.set_minor_locator(MultipleLocator(1))
 
#Now the shifting dance
ax_shifted = ax_H.twinx()
raw_spectrum = df["H_real"]
shifted_spectrum = []

N = len(raw_spectrum)
for k in range(N):
	shifted_spectrum.append( raw_spectrum[ int((k + N/2) % N) ] )

ax_shifted.plot(
    df["k"],
    shifted_spectrum,
    marker='o',
    markersize=3,
    color='tab:red',
    label='centered'
)

# Combine both legends
lines1, labels1 = ax_H.get_legend_handles_labels()
lines2, labels2 = ax_shifted.get_legend_handles_labels()

ax_H.legend(
    lines1 + lines2,
    labels1 + labels2,
    loc='best'
)
#==================================================

#fig.suptitle("10kHz sine, sampled at 100kHz")
plt.tight_layout()
fname = "idft_{:%Y%m%d_%H%M%S}.png".format(dt.now())
plt.savefig( fname , dpi=150)
#plt.legend()
plt.show()
