import struct
from PIL import Image

width, height = 640, 480
max_pixels = width * height
pixels = []

with open("data_stream.bin", "rb") as f:
    while len(pixels) < max_pixels:
        chunk = f.read(12)
        if len(chunk) < :
            break
        w0, w1, w2 = struct.unpack("<III", chunk)

        g0 =  w0        & 0xFF
        b0 = (w0 >> 8)  & 0xFF
        r0 = (w0 >> 16) & 0xFF

        g1 = (w0 >> 24) & 0xFF
        b1 =  w1        & 0xFF
        r1 = (w1 >> 8)  & 0xFF

        g2 = (w1 >> 16) & 0xFF
        b2 = (w1 >> 24) & 0xFF
        r2 =  w2        & 0xFF

        g3 = (w2 >> 8)  & 0xFF
        b3 = (w2 >> 16) & 0xFF
        r3 = (w2 >> 24) & 0xFF

        pixels.extend([
            (r0, g0, b0),
            (r1, g1, b1),
            (r2, g2, b2),
            (r3, g3, b3),
        ])

pixels = pixels[:max_pixels]

from PIL import Image
img = Image.new("RGB", (width, height))
img.putdata(pixels)
img.save("output.png")
img.show()

