# atan_lut_gen.py
import math

def generate_atan_lut(filename="atan_lut.mem"):
    lut = []
    for i in range(256):  # 0 to 255 = 0.0 to 1.0 (min/max ratio)
        ratio = i / 255.0
        angle_rad = math.atan(ratio)
        angle_deg = math.degrees(angle_rad)
        scaled_angle = round((angle_deg / 360.0) * 256)  # 360° = 256 full scale
        lut.append(scaled_angle)

    with open(filename, "w") as f:
        for val in lut:
            f.write(f"{val:02x}\n")  # Write in hex format, 2 digits

    print(f"Lookup table written to {filename}")

if __name__ == "__main__":
    generate_atan_lut()
