CXX = clang++
CXXFLAG_1= -I./include -I/prog/vcpkg/installed/x64-windows/include
CXXFLAG_LIB_1= -L/prog/vcpkg/installed/x64-windows/lib

CXXFLAGS = -shared -std=c++17 $(CXXFLAG_1) $(CXXFLAG_LIB_1) -lws2_32

TARGET = sample.dll
all: $(TARGET)

$(TARGET): sample.o
	$(CXX) $(CXXFLAGS) sample.o -o $(TARGET)

sample.o: sample.cpp
	$(CXX) $(CXXFLAGS) -c sample.cpp

clean:
	del *.o sample.dll