import struct
from PIL import Image

width, height = 640, 480
max_pixels = width * height
pixels = []

# Read the binary data stream
with open("data_stream.bin", "rb") as f:
    while True:
        chunk = f.read(12)
        if len(chunk) < 12:
            break
        w1, w2, w3 = struct.unpack("<III", chunk)

        # Unpack packed pixel data
        r0 = (w1 >> 24) & 0xFF
        g0 = (w1 >> 16) & 0xFF
        b0 = (w1 >> 8)  & 0xFF
        r1 = w1 & 0xFF

        g1 = (w2 >> 24) & 0xFF
        b1 = (w2 >> 16) & 0xFF
        r2 = (w2 >> 8)  & 0xFF
        g2 = w2 & 0xFF

        b2 = (w3 >> 24) & 0xFF
        r3 = (w3 >> 16) & 0xFF
        g3 = (w3 >> 8)  & 0xFF
        b3 = w3 & 0xFF

        pixels.extend([
            (r0, g0, b0),
            (r1, g1, b1),
            (r2, g2, b2),
            (r3, g3, b3),
        ])

        if len(pixels) >= max_pixels:
            break

pixels = pixels[:max_pixels]

# Create a PIL image and show it
img = Image.new("RGB", (width, height))
img.putdata(pixels)
img.save("output.png")
img.show()  # This opens the default image viewer
