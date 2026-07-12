import pandas as pd

# Change this if your folder is elsewhere
csv_path = "/home/anvita/Downloads/ESC-50-master/meta/esc50.csv"

df = pd.read_csv(csv_path)

print("\nUnique Categories in ESC-50:\n")
for label in sorted(df["category"].unique()):
    print(label)

print(f"\nTotal classes: {len(df['category'].unique())}")