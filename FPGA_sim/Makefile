TOP = pixel_generator

all: run

sim:
	verilator -Wall --cc $(TOP).v --exe wrapper.cpp -CFLAGS "-I/usr/local/share/verilator/include"
	make -C obj_dir -j -f V$(TOP).mk V$(TOP)

run: sim
	obj_dir/V$(TOP)
	python3 data_to_img.py
	python3 video_playback.py
	vlc output_video.mp4

clean:
	rm -rf obj_dir output_images *.bin *.png *.mp4