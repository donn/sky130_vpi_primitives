sky130_fd_sc_hd__primitives.vpi: src/*.cc src/*.h
	iverilog-vpi --name=sky130_fd_sc_hd__primitives src/*.cc
	rm *.o

.PHONY: regression
regression: 
	python3 ./regression_test.py

.PHONY: clean
clean:
	rm -rf regression
	rm -f *.vpi
