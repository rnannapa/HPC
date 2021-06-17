# Compiler details
CC=gcc
# Compiler flags
OPT = -O1
ARRAYSIZE = -DSTREAM_ARRAY_SIZE
SIZE = 100
# the build target executable:
TARGET = stream

all:$(TARGET)
$(TARGET):$(TARGET).c
	$(CC) $(OPT) $(ARRAYSIZE)=$(SIZE) -o $(TARGET) $(TARGET).c
clean:
	$(RM) $(TARGET)
