CC=gcc
N=N
SIZE=100
OPTS=L1
TARGET=quicksort
all:$(TARGET)
$(TARGET):$(TARGET).c
	$(CC) -I/opt/packages/papi/5.4.3/include/ $(TARGET).c -L/opt/packages/papi/5.4.3/lib/ -lpapi -o out$(TARGET) -D $(N)=$(SIZE) -D$(OPTS)
clean:
	$(RM) $(TARGET)
