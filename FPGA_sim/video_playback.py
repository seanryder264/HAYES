import cv2
import os

image_folder = "output_images"
output_video = "output_video.mp4"
fps = 10


images = sorted([img for img in os.listdir(image_folder) if img.endswith(".png")])


first_frame = cv2.imread(os.path.join(image_folder, images[0]))
height, width, layers = first_frame.shape

fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # or 'XVID' for AVI
video = cv2.VideoWriter(output_video, fourcc, fps, (width, height))

for image_name in images:
    img_path = os.path.join(image_folder, image_name)
    frame = cv2.imread(img_path)
    video.write(frame)

video.release()
print(f"Video saved as {output_video}")
