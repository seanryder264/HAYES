import math
import sys

output_path = sys.argv[1] if len(sys.argv) > 1 else "log_lut.mem"

def generate_log_lut(filename):
    lut = []
    for i in range(2**8):
        ratio = (i) / (2**8) 
        log2_val = math.log2(1 + ratio)
        val = int(log2_val * (2**8))
        lut.append(val)

    with open(filename, "w") as f:
        for val in lut:
            f.write(f"{val:08b}\n")

    print(f"Lookup table written to {filename}")

if __name__ == "__main__":
    generate_log_lut(output_path)
