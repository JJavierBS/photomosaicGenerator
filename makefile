TARGET = a
SRC = photomosaicGenerator.cpp
CFLAGS = `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`
all: $(TARGET)

$(TARGET): $(SRC)
	g++ $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
