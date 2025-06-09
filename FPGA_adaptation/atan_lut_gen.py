import math

def generate_atan_lut(filename="atan_lut.mem"):
    lut = []
    for i in range(256):

        ratio = i / 255
        angle_rad = math.atan(ratio)
        val = int((angle_rad / (2 * math.pi)) * 255)
        lut.append(val)

    with open(filename, "w") as f:
        for val in lut:
            f.write(f"{val:08b}\n")

    print(f"Lookup table written to {filename}")

if __name__ == "__main__":
    generate_atan_lut()

# generates the angle = atan(ratio) where ratio is in the range 0 to 1 (with 64 increments)  