import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as mplot
from matplotlib.ticker import MaxNLocator
from matplotlib.ticker import MultipleLocator
from datetime import datetime as dt
 
fname = sys.argv[1]
df = pd.read_csv( fname, skipinitialspace=True )
 
fig, (ax_bins) = mplot.subplots(1, 1, figsize=(14, 5))
 
# --- left panel: magnitude vs bin index (k) ---
ax_bins.plot(df["n"], df["y_Real"], marker='o', markersize=3, label='raw')
#ax_bins.plot(df["n"], 20 * np.log10(df["magn_hann"]), marker='o', markersize=3, color='tab:orange', label='Hanning')
ax_bins.set_title("Spectrum by bin")
ax_bins.set_xlabel("Bin index (k)")
ax_bins.set_ylabel("magn")
ax_bins.grid(True)
ax_bins.xaxis.set_major_locator(MultipleLocator(10))
ax_bins.xaxis.set_minor_locator(MultipleLocator(1))
#ax_bins.xaxis.set_major_locator(MaxNLocator(20))  # cap tick count, avoid the earlier crash
 
fig.subplots_adjust(hspace=0.4)
#fig.suptitle("10kHz sine, sampled at 100kHz")
mplot.tight_layout()
fname = fname + "_{:%Y%m%d_%H%M%S}.png".format(dt.now())
mplot.savefig( fname , dpi=150)
mplot.show()
