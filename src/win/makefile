# root directory of MATLAB installation
MATLABROOT=C:/Program Files/MATLAB/R2016b

all: main mattrans

mattrans:
	cd ../ & g++ mattrans.cpp -o build/mattrans \
    -I"${MATLABROOT}/extern/include" \
    -L"${MATLABROOT}/bin/win64" -llibmx -llibmat

main:
	cd ../graphhopper & g++ -std=c++0x -pthread main.cpp \
    -o ../build/graphhopper

mex:
	cd ../graphhopper & "${MATLABROOT}/bin/mex.bat" -largeArrayDims \
    -output graphhopper4 mex_main.cpp \
    "${MATLABROOT}/extern/lib/win64/microsoft/libut.lib"& \
    move graphhopper4.mexw64 ../build/tests/svm/graphhopper.mexw64

clean:
	del -f build/mattrans.exe
	del -f build/main.exe
