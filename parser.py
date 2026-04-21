import re
import csv
from pathlib import Path
import matplotlib.pyplot as plt

INPUT_FILE = "sensor_log.txt"
OUTPUT_DIR = Path("parsed_output")
OUTPUT_DIR.mkdir(exist_ok=True)

imu_pattern = re.compile(
    r'^(IMU(16G|32G))\(ID=(\d+)\)\s+Timestamp=(\d+)\s+'
    r'Accel=\[([-\d]+),([-\d]+),([-\d]+)\]\s+'
    r'Gyro=\[([-\d]+),([-\d]+),([-\d]+)\]\s+Temp=([-\d]+)'
)

baro_pattern = re.compile(
    r'^(BARO(07|11))\(ID=(\d+)\)\s+Timestamp=(\d+)\s+'
    r'Pressure=([-\d]+)\s+Temp=([-\d.]+)'
)

mag_pattern = re.compile(
    r'^MAG\s+Timestamp=(\d+)\s+Mag=\[([-\d]+),([-\d]+),([-\d]+)\]'
)

gps_one_line_pattern = re.compile(
    r'^GPS\s+Timestamp=(\d+)\s+Chunks=(\d+)\s+Sentence=(.*)$'
)

gps_header_pattern = re.compile(
    r'^GPS\s+Timestamp=(\d+)\s+Chunks=(\d+)$'
)

gps_sentence_line_pattern = re.compile(
    r'^Sentence=(.*)$'
)

imu_rows = []
baro_rows = []
mag_rows = []
gps_rows = []

pending_gps = None

with open(INPUT_FILE, "r", encoding="utf-8", errors="ignore") as f:
    for raw_line in f:
        line = raw_line.strip()

        if not line:
            continue

        # Skip decorative lines
        if "FLASH DUMP STOPPED" in line:
            continue

        # If previous line was "GPS Timestamp=... Chunks=..." and current line is "Sentence=..."
        if pending_gps is not None:
            sentence_match = gps_sentence_line_pattern.match(line)
            if sentence_match:
                gps_rows.append([
                    pending_gps["timestamp"],
                    pending_gps["chunks"],
                    sentence_match.group(1)
                ])
                pending_gps = None
                continue
            else:
                # Drop pending if next line was not sentence
                pending_gps = None

        imu_match = imu_pattern.match(line)
        if imu_match:
            imu_rows.append([
                imu_match.group(1),              # IMU16G / IMU32G
                imu_match.group(2),              # 16G / 32G
                int(imu_match.group(3)),         # id
                int(imu_match.group(4)),         # timestamp
                int(imu_match.group(5)),         # ax
                int(imu_match.group(6)),         # ay
                int(imu_match.group(7)),         # az
                int(imu_match.group(8)),         # gx
                int(imu_match.group(9)),         # gy
                int(imu_match.group(10)),        # gz
                int(imu_match.group(11)),        # temp
            ])
            continue

        baro_match = baro_pattern.match(line)
        if baro_match:
            baro_rows.append([
                baro_match.group(1),             # BARO07 / BARO11
                baro_match.group(2),             # 07 / 11
                int(baro_match.group(3)),        # id
                int(baro_match.group(4)),        # timestamp
                int(baro_match.group(5)),        # pressure
                float(baro_match.group(6)),      # temp
            ])
            continue

        mag_match = mag_pattern.match(line)
        if mag_match:
            mag_rows.append([
                int(mag_match.group(1)),
                int(mag_match.group(2)),
                int(mag_match.group(3)),
                int(mag_match.group(4)),
            ])
            continue

        gps_one_line_match = gps_one_line_pattern.match(line)
        if gps_one_line_match:
            gps_rows.append([
                int(gps_one_line_match.group(1)),
                int(gps_one_line_match.group(2)),
                gps_one_line_match.group(3),
            ])
            continue

        gps_header_match = gps_header_pattern.match(line)
        if gps_header_match:
            pending_gps = {
                "timestamp": int(gps_header_match.group(1)),
                "chunks": int(gps_header_match.group(2)),
            }
            continue

        print(f"Unparsed line: {line}")

# ----------------------------
# Write CSV files
# ----------------------------
with open(OUTPUT_DIR / "imu_data.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "imu_type", "sensor_range", "sensor_id", "timestamp",
        "accel_x", "accel_y", "accel_z",
        "gyro_x", "gyro_y", "gyro_z",
        "temp"
    ])
    writer.writerows(imu_rows)

with open(OUTPUT_DIR / "baro_data.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "baro_type", "baro_model", "sensor_id", "timestamp",
        "pressure", "temp"
    ])
    writer.writerows(baro_rows)

with open(OUTPUT_DIR / "mag_data.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["timestamp", "mag_x", "mag_y", "mag_z"])
    writer.writerows(mag_rows)

with open(OUTPUT_DIR / "gps_data.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["timestamp", "chunks", "sentence"])
    writer.writerows(gps_rows)

print("CSV export complete.")
print(f"IMU rows:  {len(imu_rows)}")
print(f"BARO rows: {len(baro_rows)}")
print(f"MAG rows:  {len(mag_rows)}")
print(f"GPS rows:  {len(gps_rows)}")

# ----------------------------
# Helper functions for plotting
# ----------------------------
def split_imu_rows(rows, imu_type):
    subset = [r for r in rows if r[0] == imu_type]
    return {
        "timestamp": [r[3] for r in subset],
        "ax": [r[4] for r in subset],
        "ay": [r[5] for r in subset],
        "az": [r[6] for r in subset],
        "gx": [r[7] for r in subset],
        "gy": [r[8] for r in subset],
        "gz": [r[9] for r in subset],
        "temp": [r[10] for r in subset],
    }

def split_baro_rows(rows, baro_type):
    subset = [r for r in rows if r[0] == baro_type]
    return {
        "timestamp": [r[3] for r in subset],
        "pressure": [r[4] for r in subset],
        "temp": [r[5] for r in subset],
    }

def plot_and_save(x, ys, labels, title, xlabel, ylabel, filename):
    if not x or all(len(y) == 0 for y in ys):
        print(f"Skipping {filename}, no data.")
        return

    plt.figure(figsize=(10, 5))
    for y, label in zip(ys, labels):
        plt.plot(x, y, label=label)
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / filename, dpi=150)
    plt.close()

# ----------------------------
# Generate graphs
# ----------------------------
imu16 = split_imu_rows(imu_rows, "IMU16G")
imu32 = split_imu_rows(imu_rows, "IMU32G")
baro07 = split_baro_rows(baro_rows, "BARO07")
baro11 = split_baro_rows(baro_rows, "BARO11")

plot_and_save(
    imu16["timestamp"],
    [imu16["ax"], imu16["ay"], imu16["az"]],
    ["Accel X", "Accel Y", "Accel Z"],
    "IMU16G Acceleration",
    "Timestamp",
    "Accel",
    "imu16_accel.png"
)

plot_and_save(
    imu16["timestamp"],
    [imu16["gx"], imu16["gy"], imu16["gz"]],
    ["Gyro X", "Gyro Y", "Gyro Z"],
    "IMU16G Gyroscope",
    "Timestamp",
    "Gyro",
    "imu16_gyro.png"
)

plot_and_save(
    imu16["timestamp"],
    [imu16["temp"]],
    ["Temp"],
    "IMU16G Temperature",
    "Timestamp",
    "Temperature",
    "imu16_temp.png"
)

plot_and_save(
    imu32["timestamp"],
    [imu32["ax"], imu32["ay"], imu32["az"]],
    ["Accel X", "Accel Y", "Accel Z"],
    "IMU32G Acceleration",
    "Timestamp",
    "Accel",
    "imu32_accel.png"
)

plot_and_save(
    imu32["timestamp"],
    [imu32["gx"], imu32["gy"], imu32["gz"]],
    ["Gyro X", "Gyro Y", "Gyro Z"],
    "IMU32G Gyroscope",
    "Timestamp",
    "Gyro",
    "imu32_gyro.png"
)

plot_and_save(
    imu32["timestamp"],
    [imu32["temp"]],
    ["Temp"],
    "IMU32G Temperature",
    "Timestamp",
    "Temperature",
    "imu32_temp.png"
)

plot_and_save(
    baro07["timestamp"],
    [baro07["pressure"]],
    ["Pressure"],
    "BARO07 Pressure",
    "Timestamp",
    "Pressure",
    "baro07_pressure.png"
)

plot_and_save(
    baro07["timestamp"],
    [baro07["temp"]],
    ["Temp"],
    "BARO07 Temperature",
    "Timestamp",
    "Temperature",
    "baro07_temp.png"
)

plot_and_save(
    baro11["timestamp"],
    [baro11["pressure"]],
    ["Pressure"],
    "BARO11 Pressure",
    "Timestamp",
    "Pressure",
    "baro11_pressure.png"
)

plot_and_save(
    baro11["timestamp"],
    [baro11["temp"]],
    ["Temp"],
    "BARO11 Temperature",
    "Timestamp",
    "Temperature",
    "baro11_temp.png"
)

if mag_rows:
    mag_t = [r[0] for r in mag_rows]
    mag_x = [r[1] for r in mag_rows]
    mag_y = [r[2] for r in mag_rows]
    mag_z = [r[3] for r in mag_rows]

    plot_and_save(
        mag_t,
        [mag_x, mag_y, mag_z],
        ["Mag X", "Mag Y", "Mag Z"],
        "Magnetometer",
        "Timestamp",
        "Mag",
        "mag_xyz.png"
    )

print(f"Graphs saved to: {OUTPUT_DIR.resolve()}")