import pandas as pd
import shutil
import os

CSV_PATH = "/home/anvita/Downloads/ESC-50-master/meta/esc50.csv"
AUDIO_DIR = "/home/anvita/Downloads/ESC-50-master/audio"
OUTPUT_DIR = "/home/anvita/Downloads/dataset"

CLASS_MAP = {
    "siren": "siren",
    "car_horn": "car_horn",
    "door_wood_knock": "door_knock",
    "clock_alarm": "alarm",
    "glass_breaking": "glass_break",
    "crying_baby": "baby_cry"
}

df = pd.read_csv(CSV_PATH)

os.makedirs(OUTPUT_DIR, exist_ok=True)

for esc_label, folder_name in CLASS_MAP.items():

    subset = df[df["category"] == esc_label]

    if subset.empty:
        print(f"{esc_label} not found")
        continue

    out_folder = os.path.join(OUTPUT_DIR, folder_name)
    os.makedirs(out_folder, exist_ok=True)

    for _, row in subset.iterrows():

        src = os.path.join(AUDIO_DIR, row["filename"])
        dst = os.path.join(out_folder, row["filename"])

        shutil.copy(src, dst)

    print(f"Copied {len(subset)} files -> {folder_name}")

print("\nDataset created successfully!")