TOP = pixel_generator

all: sim

sim:
	python3 atan_lut_gen.py
	verilator -Wall --cc $(TOP).v --exe wrapper.cpp
	make -C obj_dir -j -f V$(TOP).mk V$(TOP)
	python3 data_to_img.py

run: sim
	obj_dir/V$(TOP)

clean:
	rm -rf obj_dir *.bin *.png *.mem *.ppm
