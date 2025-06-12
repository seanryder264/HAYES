import numpy as np
import matplotlib.pyplot as plt

# Load binary data
data = np.fromfile("adc_dump.bin", dtype=np.uint16)

# Sampling info
fs = 5000000  # Sample rate in Hz
t = np.arange(len(data)) / fs

# Plot
plt.plot(t, data)
plt.xlabel("Time (s)")
plt.ylabel("ADC Value")
plt.title("ADC Time Series")
plt.grid(True)
plt.show()