# root directory of MATLAB installation
MATLABROOT="C:/Program Files/MATLAB/R2015a"

all: mattrans

mattrans:
	cd ../; g++ mattrans.cpp -o build/mattrans \
    -I${MATLABROOT}/extern/include \
    -L${MATLABROOT}/bin/win64 -llibmx -llibmat


clean:
	rm -f build/mattrans.exe
