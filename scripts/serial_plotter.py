"""
serial_plotter.py — Live tremor data visualizer
Reads CSV lines from the device over serial and plots in real time.

Usage:
    pip install pyserial matplotlib
    python serial_plotter.py --port COM3 --baud 115200      # Windows
    python serial_plotter.py --port /dev/ttyUSB0            # Linux/Mac

Expected serial format (set SERIAL_DEBUG=true in config.h):
    ts_ms,rms,lt_avg,state
"""

import argparse
import threading
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

WINDOW = 400  # samples to display

ts    = deque(maxlen=WINDOW)
rms   = deque(maxlen=WINDOW)
ltavg = deque(maxlen=WINDOW)
state_log  = deque(maxlen=WINDOW)
current_state = ["STABLE"]

STATE_COLORS = {
    "STABLE":           "#2ecc71",
    "MILD_TREMOR":      "#f39c12",
    "SEVERE_TREMOR":    "#e74c3c",
    "FATIGUE_ELEVATED": "#9b59b6",
}

def serial_reader(port, baud):
    """Background thread: reads serial and populates deques."""
    try:
        ser = serial.Serial(port, baud, timeout=1)
        print(f"[serial] Connected to {port} @ {baud}")
        while True:
            try:
                line = ser.readline().decode("utf-8", errors="replace").strip()
                parts = line.split(",")
                if len(parts) == 4:
                    try:
                        ts.append(int(parts[0]))
                        rms.append(float(parts[1]))
                        ltavg.append(float(parts[2]))
                        current_state[0] = parts[3].strip()
                        state_log.append(current_state[0])
                    except ValueError:
                        pass  # header line or partial packet
            except Exception:
                pass
    except serial.SerialException as e:
        print(f"[serial] ERROR: {e}")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", default="COM3")
    parser.add_argument("--baud", type=int, default=115200)
    args = parser.parse_args()

    t = threading.Thread(target=serial_reader, args=(args.port, args.baud), daemon=True)
    t.start()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(13, 7), sharex=False)
    fig.suptitle("Wrist Tremor Monitor — Live Feed", fontsize=14)

    line_rms,  = ax1.plot([], [], color="#2980b9", lw=1.5, label="Tremor RMS (m/s²)")
    line_ltavg,= ax1.plot([], [], color="#e67e22", lw=1.5, ls="--", label="Long-term Avg")
    ax1.axhline(0.5, color="#f39c12", ls=":", alpha=0.7, label="Mild threshold (0.5)")
    ax1.axhline(1.5, color="#e74c3c", ls=":", alpha=0.7, label="Severe threshold (1.5)")
    ax1.set_ylim(0, 3)
    ax1.set_ylabel("RMS Accel (m/s²)")
    ax1.legend(loc="upper right", fontsize=8)
    state_txt = ax1.text(0.01, 0.92, "", transform=ax1.transAxes,
                          fontsize=11, fontweight="bold")

    # State timeline (bottom plot)
    scat = ax2.scatter([], [], c=[], s=8, marker="|")
    ax2.set_ylim(-0.5, 3.5)
    ax2.set_yticks([0,1,2,3])
    ax2.set_yticklabels(["STABLE","MILD","SEVERE","FATIGUE"], fontsize=8)
    ax2.set_ylabel("State")
    ax2.set_xlabel("Sample index")

    STATE_NUM = {"STABLE":0,"MILD_TREMOR":1,"SEVERE_TREMOR":2,"FATIGUE_ELEVATED":3}

    def update(_frame):
        xs = list(range(len(rms)))
        line_rms.set_data(xs, list(rms))
        line_ltavg.set_data(xs, list(ltavg))
        ax1.set_xlim(0, max(WINDOW, len(rms)))

        cs = current_state[0]
        color = STATE_COLORS.get(cs, "gray")
        state_txt.set_text(f"State: {cs}")
        state_txt.set_color(color)

        # State scatter
        s_xs = list(range(len(state_log)))
        s_ys = [STATE_NUM.get(s, 0) for s in state_log]
        s_cs = [STATE_COLORS.get(s, "gray") for s in state_log]
        ax2.cla()
        ax2.scatter(s_xs, s_ys, c=s_cs, s=8, marker="|")
        ax2.set_ylim(-0.5, 3.5)
        ax2.set_yticks([0,1,2,3])
        ax2.set_yticklabels(["STABLE","MILD","SEVERE","FATIGUE"], fontsize=8)
        ax2.set_ylabel("State")
        ax2.set_xlabel("Sample index")

        return line_rms, line_ltavg, state_txt

    ani = animation.FuncAnimation(fig, update, interval=50, blit=False)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
