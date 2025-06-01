import math

def float_to_q15(angle_radians):
    # Scale from [0, pi/4] to [0, 16384]
    return int(round((angle_radians / (math.pi / 2)) * 32768))

with open("log_rtl/atan_lut_256.mem", "w") as f:
    for i in range(256):
        # Compute ratio from 0 to ~1 (256 steps)
        ratio = i / 255.0
        angle_rad = math.atan(ratio)
        q15_val = float_to_q15(angle_rad)
        # Format as 4-digit hex (16 bits)
        f.write("{:04X}\n".format(q15_val))
