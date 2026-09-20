import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


# Load the filtered output
df = pd.read_csv("filtered_tones3.csv", header=0, names=["n", "y", "col3"])

# Grab the steady-state window: n = 32 to 47 (16 samples, one full period)
window = df[(df["n"] >= 32) & (df["n"] <= 47)]["y"].to_numpy()

print("Window samples:")
print(window)
print(f"Window length: {len(window)}")

# 16-point DFT
Y = np.fft.fft(window)

Fs = 8000
N = len(window)
freqs = np.fft.fftfreq(N, d=1/Fs)

print("\nBin | Freq (Hz) | Magnitude | Phase (rad)")
for k in range(N):
    mag = np.abs(Y[k])
    phase = np.angle(Y[k])
    print(f"{k:3d} | {freqs[k]:9.1f} | {mag:9.4f} | {phase:7.4f}")

print("\n--- Your three tones ---")
for k, f in [(1, 500), (3, 1500), (6, 3000)]:
    print(f"Bin {k} ({f} Hz): magnitude = {np.abs(Y[k]):.4f}")

###############################################

fig, ax = plt.subplots(figsize=(12, 5))

ax.stem(freqs[:N//2+1], np.abs(Y)[:N//2+1], basefmt=" ")
ax.set_xlabel("Frequency (Hz)")
ax.set_ylabel("Magnitude")
ax.set_title("Filtered signal — DFT bin magnitudes (n=32 to 47 window)")
ax.grid(True, alpha=0.3)

# Annotate the three tone bins
for k, f in [(1, 500), (3, 1500), (6, 3000)]:
    ax.annotate(f"{np.abs(Y[k]):.2f}", (freqs[k], np.abs(Y[k])),
                textcoords="offset points", xytext=(0, 8), ha="center")

plt.tight_layout()
plt.savefig("filtered_spectrum.png", dpi=120)
plt.show()
