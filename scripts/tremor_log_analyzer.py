"""
tremor_log_analyzer.py — Post-session CSV analysis and report
Analyzes SD card logs or serial-captured CSV files.

Usage:
    pip install pandas matplotlib
    python tremor_log_analyzer.py --file tremor_log.csv [--out report.png]
"""

import argparse
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

STATE_COLORS = {
    "STABLE":           "#2ecc71",
    "MILD_TREMOR":      "#f39c12",
    "SEVERE_TREMOR":    "#e74c3c",
    "FATIGUE_ELEVATED": "#9b59b6",
}
STATE_NUM = {"STABLE":0,"MILD_TREMOR":1,"SEVERE_TREMOR":2,"FATIGUE_ELEVATED":3}

def main():
    p = argparse.ArgumentParser()
    p.add_argument("--file", required=True, help="CSV log file path")
    p.add_argument("--out",  default="tremor_report.png", help="Output image path")
    args = p.parse_args()

    df = pd.read_csv(args.file)
    df.columns = [c.strip() for c in df.columns]

    # Accept either column name style
    col_map = {
        "timestamp_ms": "ts", "ts_ms": "ts",
        "rms_ms2": "rms", "rms": "rms",
        "long_term_avg": "ltavg", "lt_avg": "ltavg",
        "state": "state"
    }
    df.rename(columns={k:v for k,v in col_map.items() if k in df.columns}, inplace=True)
    df["time_s"] = df["ts"] / 1000.0

    fig, axes = plt.subplots(3, 1, figsize=(14, 11), sharex=True)
    fig.suptitle(f"Tremor Session Report — {args.file}", fontsize=13)

    # Plot 1: RMS
    ax1 = axes[0]
    ax1.plot(df["time_s"], df["rms"], color="#2980b9", lw=1, label="Tremor RMS")
    ax1.axhline(0.5, color="#f39c12", ls="--", alpha=0.7, label="Mild (0.5)")
    ax1.axhline(1.5, color="#e74c3c", ls="--", alpha=0.7, label="Severe (1.5)")
    ax1.fill_between(df["time_s"], df["rms"], alpha=0.15, color="#2980b9")
    ax1.set_ylabel("RMS (m/s²)")
    ax1.set_title("Tremor Intensity")
    ax1.legend(fontsize=8)

    # Plot 2: Long-term avg (fatigue trend)
    ax2 = axes[1]
    ax2.plot(df["time_s"], df["ltavg"], color="#e67e22", lw=1.5)
    ax2.axhline(0.8, color="#9b59b6", ls="--", alpha=0.7, label="Fatigue threshold (0.8)")
    ax2.set_ylabel("Long-term Avg")
    ax2.set_title("Fatigue Trend (Rolling Average)")
    ax2.legend(fontsize=8)

    # Plot 3: State timeline
    ax3 = axes[2]
    s_ys = df["state"].map(STATE_NUM).fillna(0)
    s_cs = df["state"].map(STATE_COLORS).fillna("gray")
    ax3.scatter(df["time_s"], s_ys, c=s_cs, s=5, marker="|")
    ax3.set_yticks([0,1,2,3])
    ax3.set_yticklabels(["STABLE","MILD","SEVERE","FATIGUE"])
    ax3.set_xlabel("Time (s)")
    ax3.set_title("Device State Timeline")
    patches = [mpatches.Patch(color=c, label=s) for s,c in STATE_COLORS.items()]
    ax3.legend(handles=patches, fontsize=7, loc="upper right")

    plt.tight_layout()
    plt.savefig(args.out, dpi=150, bbox_inches="tight")
    print(f"[Saved] {args.out}")
    plt.show()

    # Summary stats
    dur = df["time_s"].max()
    print(f"\n=== Session Summary ===")
    print(f"Duration        : {dur:.1f} s ({dur/60:.1f} min)")
    print(f"Mean RMS        : {df['rms'].mean():.4f} m/s²")
    print(f"Peak RMS        : {df['rms'].max():.4f} m/s²")
    print(f"Final fatigue   : {df['ltavg'].iloc[-1]:.4f} m/s²")
    print(f"\nTime in each state:")
    counts = df["state"].value_counts(normalize=True).mul(100).round(1)
    for s, pct in counts.items():
        print(f"  {s:<20}: {pct:.1f}%")

if __name__ == "__main__":
    main()
