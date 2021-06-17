TARGET = dgemm

all:$(TARGET)
$(TARGET):$(TARGET).c
	icc -o $(TARGET) $(TARGET).c -I /opt/packages/OpenBLAS/0.2.19/include/ -L /opt/packages/OpenBLAS/0.2.19/lib -lopenblas
clean:
	$(RM) $(TARGET)

