import numpy as np
import pandas as pd

df = pd.read_csv("h.csv", header=0, names=["k", "h_real", "h_imag", "h_win_Real", "h_win_Imag"])

h_unwindowed = df["h_real"].to_numpy() + 1j * df["h_imag"].to_numpy()
h_windowed = df["h_win_Real"].to_numpy() + 1j * df["h_win_Imag"].to_numpy()

H_unwindowed = np.fft.fft(h_unwindowed)
H_windowed = np.fft.fft(h_windowed)

Fs = 8000
N = 64
freqs = np.fft.fftfreq(N, d=1/Fs)

print(f"{'Bin':>4} | {'Freq(Hz)':>9} | {'Unwindowed |H|':>15} | {'Windowed |H|':>13}")
for k in range(15):  # focus on bins near and past the transition edge
    print(f"{k:4d} | {freqs[k]:9.1f} | {np.abs(H_unwindowed[k]):15.4f} | {np.abs(H_windowed[k]):13.4f}")

print("\n--- Deep stopband bins, for comparison ---")
for k in [12, 24]:
    print(f"{k:4d} | {freqs[k]:9.1f} | {np.abs(H_unwindowed[k]):15.4f} | {np.abs(H_windowed[k]):13.4f}")
