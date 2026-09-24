import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as mplot
from matplotlib.ticker import MaxNLocator
from matplotlib.ticker import MultipleLocator
from datetime import datetime as dt
 
fname = sys.argv[1]
df = pd.read_csv( fname, skipinitialspace=True )
 
fig, (ax_ampl, ax_ph1, ax_ph2, ax_ph3) = mplot.subplots(4, 1, figsize=(14, 12))
 
#COMPOSITE SIGNAL
ax_ampl.plot(df["index"], df["x[n]"], marker='o', markersize=3, label='raw')
ax_ampl.set_xlabel("n")
ax_ampl.set_ylabel("Amplitude")
ax_ampl.set_title("signal")
ax_ampl.grid(True)
#ax_ampl.xaxis.set_major_locator(MultipleLocator(2))
#ax_ampl.xaxis.set_minor_locator(MultipleLocator(1))
#ax_ampl.xaxis.set_major_locator(MaxNLocator(20))  # cap tick count, avoid the earlier crash
 
####################################################################

#COMPONENT1
ax_ph1.plot(df["index"], df["phase1"], marker='o', markersize=3, color='tab:orange')
ax_ph1.set_xlabel("n")
ax_ph1.set_ylabel("phase1")
ax_ph1.set_title("Phase")
ax_ph1.grid(True)
#ax_ph1.xaxis.set_major_locator(MultipleLocator(2))
#ax_ph1.xaxis.set_minor_locator(MultipleLocator(1))
#-------------------------------------------------
ax_y1 = ax_ph1.twinx()
ax_y1.plot(df["index"], df["x1"], marker='o', markersize=3, label='raw')
 
###################################################################

#COMPONENT2
ax_ph2.plot(df["index"], df["phase2"], marker='o', markersize=3, color='tab:orange')
ax_ph2.set_xlabel("n")
ax_ph2.set_ylabel("phase2")
ax_ph2.set_title("Phase")
ax_ph2.grid(True)
#ax_ph2.xaxis.set_major_locator(MultipleLocator(2))
#ax_ph2.xaxis.set_minor_locator(MultipleLocator(1))
#-------------------------------------------------
ax_y2 = ax_ph2.twinx()
ax_y2.plot(df["index"], df["x2"], marker='o', markersize=3, label='raw')

#####################################################################

#COMPONENT3
ax_ph3.plot(df["index"], df["phase3"], marker='o', markersize=3, color='tab:orange')
ax_ph3.set_xlabel("n")
ax_ph3.set_ylabel("phase3")
ax_ph3.set_title("Phase")
ax_ph3.grid(True)
#ax_ph3.xaxis.set_major_locator(MultipleLocator(2))
#ax_ph3.xaxis.set_minor_locator(MultipleLocator(1))
#-------------------------------------------------
ax_y3 = ax_ph3.twinx()
ax_y3.plot(df["index"], df["x3"], marker='o', markersize=3, label='raw')

####################################################################

fig.subplots_adjust(hspace=0.4)
#fig.suptitle("10kHz sine, sampled at 100kHz")
mplot.tight_layout()
fname = "tones3_{:%Y%m%d_%H%M%S}.png".format(dt.now())
mplot.savefig( fname , dpi=150)
#mplot.legend()
mplot.show()
