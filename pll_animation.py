import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# --------------------------------------------------
# Simulation
# --------------------------------------------------

fs = 1000
duration = 10

t = np.arange(0, duration, 1 / fs)
N = len(t)

f_ref = 1.0
f_osc = 0.9

phase_ref = 0.0
phase_osc = np.deg2rad(60)

Kp = 0.5
Ki = 0.05
integrator = 0.0

phase_error = np.zeros(N)
freq_history = np.zeros(N)
ref_history = np.zeros(N)
osc_history = np.zeros(N)

for n in range(N):

    # Phase detector
    error = np.angle(
        np.exp(1j * (phase_ref - phase_osc))
    )

    phase_error[n] = error

    # PI loop filter
    integrator += Ki * error
    correction = Kp * error + integrator

    # VCO / NCO frequency correction
    f_osc += correction

    # Advance both oscillators
    phase_ref += 2 * np.pi * f_ref / fs
    phase_osc += 2 * np.pi * f_osc / fs

    # Keep phases bounded
    phase_ref = np.angle(np.exp(1j * phase_ref))
    phase_osc = np.angle(np.exp(1j * phase_osc))

    ref_history[n] = phase_ref
    osc_history[n] = phase_osc
    freq_history[n] = f_osc


# --------------------------------------------------
# Unit-circle animation
# --------------------------------------------------

fig, ax = plt.subplots(figsize=(7, 7))

ax.set_aspect("equal")
ax.set_xlim(-1.25, 1.25)
ax.set_ylim(-1.25, 1.25)

ax.set_xlabel("cos(phase)")
ax.set_ylabel("sin(phase)")
ax.set_title("PLL — Oscillator Chasing Reference")
ax.grid()

# Unit circle
theta = np.linspace(0, 2 * np.pi, 500)
ax.plot(np.cos(theta), np.sin(theta), linestyle="--")

# Phasors
ref_line, = ax.plot([], [], linewidth=2, label="Reference")
osc_line, = ax.plot([], [], linewidth=2, label="PLL oscillator")

ref_dot, = ax.plot([], [], marker="o", markersize=8)
osc_dot, = ax.plot([], [], marker="o", markersize=8)

# Readouts
time_text = ax.text(-1.18, 1.12, "")
error_text = ax.text(-1.18, 1.02, "")
freq_text = ax.text(-1.18, 0.92, "")

ax.legend(loc="lower left")


# --------------------------------------------------
# Animation functions
# --------------------------------------------------

def init():

    ref_line.set_data([], [])
    osc_line.set_data([], [])

    ref_dot.set_data([], [])
    osc_dot.set_data([], [])

    time_text.set_text("")
    error_text.set_text("")
    freq_text.set_text("")

    return (
        ref_line,
        osc_line,
        ref_dot,
        osc_dot,
        time_text,
        error_text,
        freq_text
    )


def update(n):

    pr = ref_history[n]
    po = osc_history[n]

    xr = np.cos(pr)
    yr = np.sin(pr)

    xo = np.cos(po)
    yo = np.sin(po)

    # Draw phasors
    ref_line.set_data([0, xr], [0, yr])
    osc_line.set_data([0, xo], [0, yo])

    # Draw tips
    ref_dot.set_data([xr], [yr])
    osc_dot.set_data([xo], [yo])

    # Readouts
    time_text.set_text(
        f"t = {t[n]:.2f} s"
    )

    error_text.set_text(
        f"phase error = {np.rad2deg(phase_error[n]):+.2f}°"
    )

    freq_text.set_text(
        f"oscillator = {freq_history[n]:.4f} Hz"
    )

    return (
        ref_line,
        osc_line,
        ref_dot,
        osc_dot,
        time_text,
        error_text,
        freq_text
    )


# About 30 frames/sec
frame_step = fs // 30

frames = np.arange(
    0,
    N,
    frame_step
)

ani = FuncAnimation(
    fig,
    update,
    frames=frames,
    init_func=init,
    interval=1000 * frame_step / fs,
    blit=True
)

plt.show()
