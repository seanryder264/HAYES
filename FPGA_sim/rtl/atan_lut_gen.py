import sys
import math

def generate_atan_lut(filename):
    lut = []
    for i in range(256): 
        ratio = i / 255
        angle_rad = math.atan(ratio)
        val = round((angle_rad / (2 * math.pi)) * (2**16))
        lut.append(val)

    with open(filename, "w") as f:
        for val in lut:
            f.write(f"{val:016b}\n")

    print(f"Lookup table written to {filename}")

if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else "atan_lut.mem"
    generate_atan_lut(filename)
