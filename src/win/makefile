# root directory of MATLAB installation
MATLABROOT="C:/Program Files/MATLAB/R2015a"
BOOSTROOT="C:/Users/RKN/Documents/Studie/Blokke/2015-2016_Blok_4/bachelor/src/boost"

all: main mattrans

mattrans:
	cd ../; g++ mattrans.cpp -o build/mattrans \
    -I${MATLABROOT}/extern/include \
    -L${MATLABROOT}/bin/win64 -llibmx -llibmat

main:
	cd ../graphhopper; g++ -std=c++0x -pthread main.cpp \
    -o ../build/graphhopper \
    -I${BOOSTROOT}

clean:
	rm -f build/mattrans.exe
	rm -f build/main.exe
