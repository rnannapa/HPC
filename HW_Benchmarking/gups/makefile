CC=mpicc
OPT = -O1
TARGET = gups_vanilla

all:$(TARGET)
$(TARGET):$(TARGET).c
	$(CC) $(OPT) $(TARGET).c -o $(TARGET)
clean:
	$(RM) $(TARGET)

