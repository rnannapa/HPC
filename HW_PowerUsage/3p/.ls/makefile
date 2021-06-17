CC=gcc
N=N
SIZE=100
OPTS=L
TARGET=quicksort
all:$(TARGET)
$(TARGET):$(TARGET).c
	$(CC) -I/opt/packages/papi/5.4.3/include/ $(TARGET).c -L/opt/packages/papi/5.4.3/lib/ -lpapi -o $(TARGET) -D $(N)=$(SIZE) -D$(OPTS)
clean:
	$(RM) $(TARGET)
