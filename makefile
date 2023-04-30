CC = g++
CFLAGS = -Wall -W 
HEADERS = vehicle.h casualty.h hospital.h instance.h 
OBJ = vehicle.o casualty.o hospital.o instance.o main.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

CSTPSolver: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -rf *.o CSTPSolver