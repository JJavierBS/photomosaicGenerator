TARGET = a
SRC = main.cpp photomosaicGenerator.cpp

CFLAGS = -fPIC `pkg-config --cflags opencv4 Qt5Widgets`
LIBS = `pkg-config --libs opencv4 Qt5Widgets`

all: $(TARGET)

$(TARGET): $(SRC)
	g++ $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

