import os

dataset_path = "dataset"

print("\nNumber of clips per class:\n")

total = 0

for folder in sorted(os.listdir(dataset_path)):
    folder_path = os.path.join(dataset_path, folder)

    if os.path.isdir(folder_path):
        count = len([
            f for f in os.listdir(folder_path)
            if f.lower().endswith(".wav")
        ])

        total += count
        print(f"{folder:15s} : {count}")

print("\n----------------------")
print(f"Total clips : {total}")