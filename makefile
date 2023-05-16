CC = g++
CFLAGS = -Wall -W 
HEADERS = vehicle.h casualty.h hospital.h instance.h solver.h
OBJ = vehicle.o casualty.o hospital.o instance.o solver.o main.o

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

CSTPSolver: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -rf *.o CSTPSolver