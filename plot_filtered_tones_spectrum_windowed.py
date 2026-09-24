import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

Fs = 8000

# --- Load the FILTERED output ---
df = pd.read_csv("filtered_tones3.csv", header=0, names=["n", "y_Real", "y_Imag"])
window = df[(df["n"] >= 32) & (df["n"] <= 47)]["y_Real"].to_numpy()

print("Filtered window samples:")
print(window)
print(f"Window length: {len(window)}")

N = len(window)
freqs = np.fft.fftfreq(N, d=1/Fs)
Y = np.fft.fft(window)

print("\nBin | Freq (Hz) | Magnitude | Phase (rad)")
for k in range(N):
    print(f"{k:3d} | {freqs[k]:9.1f} | {np.abs(Y[k]):9.4f} | {np.angle(Y[k]):7.4f}")

print("\n--- Filtered tone bins ---")
for k, f in [(1, 500), (3, 1500), (6, 3000)]:
    print(f"Bin {k} ({f} Hz): magnitude = {np.abs(Y[k]):.4f}")

# --- Load the ORIGINAL (unfiltered) signal ---
# --- Load the ORIGINAL (unfiltered) signal ---
df_orig = pd.read_csv(
    "tones3.csv",
    header=0,
    names=["n", "x", "sig_win", "phase1", "phase2", "phase3", "x1", "x2", "x3"]
)
df_orig["n"] = pd.to_numeric(df_orig["n"], errors="coerce")
df_orig["x"] = pd.to_numeric(df_orig["x"], errors="coerce")
df_orig = df_orig.dropna(subset=["n", "x"])

window_orig = df_orig[(df_orig["n"] >= 32) & (df_orig["n"] <= 47)]["x"].to_numpy()
print(f"Original window length: {len(window_orig)}")  # sanity check — should print 16

X = np.fft.fft(window_orig)

print("\n--- Original tone bins ---")
for k, f in [(1, 500), (3, 1500), (6, 3000)]:
    print(f"Bin {k} ({f} Hz): magnitude = {np.abs(X[k]):.4f}")

# --- Gain comparison ---
print("\n--- Gain (filtered / original) at each tone ---")
for k, f in [(1, 500), (3, 1500), (6, 3000)]:
    gain = np.abs(Y[k]) / np.abs(X[k])
    print(f"{f:5d} Hz: |X|={np.abs(X[k]):.4f}  |Y|={np.abs(Y[k]):.4f}  gain={gain:.4f}")

# --- Overlay plot: original vs filtered ---
fig, ax = plt.subplots(figsize=(12, 5))
half = N // 2 + 1

ax.stem(freqs[:half], np.abs(X)[:half], linefmt="C1-", markerfmt="C1o", basefmt=" ", label="Original")
ax.stem(freqs[:half], np.abs(Y)[:half], linefmt="C0-", markerfmt="C0o", basefmt=" ", label="Filtered")

ax.set_xlabel("Frequency (Hz)")
ax.set_ylabel("Magnitude")
ax.set_title("Original vs Filtered — DFT bin magnitudes")
ax.legend()
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("spectrum_comparison.png", dpi=120)
plt.show()
