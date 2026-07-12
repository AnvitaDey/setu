from pydub import AudioSegment
import os

# Change this to "dataset/gunisha" when needed
FOLDER = "/home/anvita/Desktop/starlet/dataset/gunisha"

CLIP_LENGTH_MS = 30 * 1000   # 30 seconds

for filename in os.listdir(FOLDER):

    if not filename.lower().endswith(".wav"):
        continue

    path = os.path.join(FOLDER, filename)

    audio = AudioSegment.from_wav(path)

    base = os.path.splitext(filename)[0]

    clip_no = 1

    for start in range(0, len(audio), CLIP_LENGTH_MS):

        clip = audio[start:start + CLIP_LENGTH_MS]

        # Ignore tiny leftover pieces (<15 sec)
        if len(clip) < 15000:
            continue

        out_name = f"{base}_{clip_no}.wav"
        out_path = os.path.join(FOLDER, out_name)

        clip.export(out_path, format="wav")

        print("Saved:", out_name)

        clip_no += 1

print("Done!")