CC=g++
CFLAGS=-Wall -std=c++11
TARGET=ALSP
OBJ= main.o modelo.o


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.cpp Modelo.hpp
	$(CC) $(CFLAGS) -c $<

model.o: modelo.cpp Modelo.hpp
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm *.txt

run:
	./$(TARGET) 