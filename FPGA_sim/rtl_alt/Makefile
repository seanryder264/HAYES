TOP = pixel_generator
PIPE = pixel_pipe

all: sim

# Build simulation executable
sim:
	touch coloured_function.ppm
	python3 atan_lut_gen.py
	verilator -Wall --cc $(TOP).v --exe wrapper.cpp
	make -C obj_dir -j -f V$(TOP).mk V$(TOP)

# Run only the simulation (writes to pipe)
run: clean sim
	./obj_dir/V$(TOP)

clean:
	rm -rf obj_dir *.bin *.png *.mem *.ppm *.txt
