from pathlib import Path

# Get the root directory of your app (folder containing this .py file)
root_dir = Path(__file__).resolve().parent

# Paths
file_path = root_dir / "data.txt"
log_path = root_dir / "log.txt"

# Read the file (optional)
with open(file_path, "r", encoding="utf-8") as f:
    data = f.read()

value = 50
c = 0
atZero = False
thisRound = False

with open(file_path, "r", encoding="utf-8") as f, open(log_path, "w", encoding="utf-8") as log:
    for line in f:
        line = line.strip()   # remove newline + spaces

        sign = 1 if line[0] == "R" else -1
        num = int(line[1:])

        rest = num % 100

        fullRotations = num // 100
        c += fullRotations
        
        value = value + sign * rest
        
        if value < 0:
            if not atZero:
                c += 1
            value = value + 100
        elif value > 100:
            c += 1
            value = value % 100
        elif value == 0 or value == 100:
            value = 0
            atZero = True
            thisRound = True
            c += 1
            
        if not thisRound:
            atZero = False
            
        thisRound = False
        # BUILD THE LOG LINE
        log_line = f"Processing: {line}  rest: {rest}  value: {value}  c: {c}  fullRotations: {fullRotations}"

        # PRINT TO CONSOLE
        print(log_line)

        # WRITE TO LOG FILE
        log.write(log_line + "\n")

# Final count
print(c)
