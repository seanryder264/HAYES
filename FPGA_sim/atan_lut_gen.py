import math

def generate_atan_lut(filename="atan_lut.mem"):
    lut = []
    for i in range(1024):
        ratio = i / 1023
        angle_rad = math.atan(ratio)
        angle_deg = angle_rad * 180 / math.pi
        lut.append(angle_deg)

    with open(filename, "w") as f:
        for val in lut:
            f.write(f"{int(val):06b}\n")

    print(f"Lookup table written to {filename}")

if __name__ == "__main__":
    generate_atan_lut()

# generates the angle = atan(ratio) where ratio is in the range 0 to 1 (with 64 increments)  