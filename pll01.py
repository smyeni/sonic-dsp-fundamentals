import numpy as np
import matplotlib.pyplot as plt

# Simulation parameters
fs = 1000 # simulation sample rate
duration = 10 # seconds

t = np.arange(0, duration, 1/fs)

# Two oscillators
f_ref = 1.0
f_osc = 0.9

phase_ref = 2 * np.pi * f_ref * t
phase_osc = 2 * np.pi * f_osc * t + np.deg2rad(60)

# Phase difference
phase_error = np.angle(np.exp(1j * (phase_ref - phase_osc)))

# Unit-circle coordinates
x_ref = np.cos(phase_ref)
y_ref = np.sin(phase_ref)

x_osc = np.cos(phase_osc)
y_osc = np.sin(phase_osc)

# --------------------------------------------------
# Plot 1: phase difference
# --------------------------------------------------

plt.figure()
plt.plot(t, np.rad2deg(phase_error))
plt.xlabel("Time (s)")
plt.ylabel("Phase difference (degrees)")
plt.title("Phase Difference")
plt.grid()
plt.show()

# --------------------------------------------------
# Plot 2: trajectories on the unit circle
# --------------------------------------------------

plt.figure()

plt.plot(x_ref, y_ref, label="Reference")
plt.plot(x_osc, y_osc, label="Oscillator")

plt.axis("equal")
plt.xlabel("cos(phase)")
plt.ylabel("sin(phase)")
plt.title("Two Rotating Phasors")
plt.grid()
plt.legend()
plt.show()
