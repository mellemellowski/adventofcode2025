from pathlib import Path

# Get the root directory of your app (folder containing this .py file)
root_dir = Path(__file__).resolve().parent

# Path to the file at the app's root
file_path = root_dir / "data.txt"

# Read the file (optional)
with open(file_path, "r", encoding="utf-8") as f:
    data = f.read()

print(data)

value = 50
c = 0

with open(file_path, "r", encoding="utf-8") as f:
    for line in f:
        line = line.strip()   # remove newline + spaces

        sign = 1 if line[0] == "R" else -1
        num = int(line[1:])

        rest = num % 100
        #c += num // 100
        
        value = value + sign * rest

        if value < 0:
            value = value + 100
        elif value > 100:
            value = value % 100
        elif value == 0 or value == 100:
            c += 1

        print("Processing:", line, " rest:", rest, " value:", value, " c:", c)

print(c)