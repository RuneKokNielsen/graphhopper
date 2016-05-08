# root directory of MATLAB installation
MATLABROOT="C:/Program Files/MATLAB/R2016a"

all: main mattrans

mattrans:
	cd ../; g++ mattrans.cpp -o build/mattrans \
    -I${MATLABROOT}/extern/include \
    -L${MATLABROOT}/bin/win64 -llibmx -llibmat

main:
	cd ../graphhopper; g++ -std=c++0x -pthread main.cpp \
    -o ../build/graphhopper

mex:
	cd ../graphhopper; ${MATLABROOT}/bin/mex.bat -largeArrayDims \
    mex_main.cpp; \
    mv mex_main.mexw64 ../build/mex_main.mexw64

clean:
	rm -f build/mattrans.exe
	rm -f build/main.exe
