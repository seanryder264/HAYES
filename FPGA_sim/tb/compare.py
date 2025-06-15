import sys

def read_ppm_ascii(filename):
    with open(filename, "rb") as f:
        # Read magic number
        magic = f.readline().strip()
        if magic != b'P3':
            raise ValueError("Only P3 ASCII PPM format is supported.")

        # Skip comments
        line = f.readline()
        while line.startswith(b'#'):
            line = f.readline()

        # Read width and height
        width, height = map(int, line.strip().split())

        # Read max value (usually 255)
        max_val = int(f.readline().strip())
        if max_val != 255:
            raise ValueError("Only max value of 255 is supported.")

        # Read pixel values (space-separated ASCII integers)
        pixel_values = []
        for line in f:
            line = line.strip()
            if line and not line.startswith(b'#'):
                pixel_values += list(map(int, line.split()))

        if len(pixel_values) != width * height * 3:
            raise ValueError("PPM pixel data does not match expected size.")

        return pixel_values

def sum_values(data):
    return sum(data)

def compute_percentage_error(ref_data, test_data):
    sum_ref = sum_values(ref_data)
    sum_test = sum_values(test_data)
    diff = abs(sum_test - sum_ref)

    percent_error = (diff / sum_ref) * 100 if sum_ref != 0 else 0
    return percent_error

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python compare_ppm.py correct.ppm test.ppm")
        sys.exit(1)

    ref_data = read_ppm_ascii(sys.argv[1])
    test_data = read_ppm_ascii(sys.argv[2])

    error = compute_percentage_error(ref_data, test_data)
    print(f"Percentage error: {error:.4f}%")
