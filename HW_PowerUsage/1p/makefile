CC=icc
OPTS=L
SIZE=3000
TARGET1=mklMul_PAPI
TARGET2=matmul_PAPI
TARGET3=mklMul_Power
TARGET4=matmul_Power
all:$(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)

$(TARGET1):$(TARGET1).c
	$(CC) -mkl -I/opt/packages/papi/5.4.3/include/ -O0 $(TARGET1).c -L/opt/packages/papi/5.4.3/lib/ -lpapi -o $(TARGET1).out -D$(OPTS) -D mdim=$(SIZE) -D ndim=$(SIZE) -D kdim=$(SIZE)

$(TARGET2):$(TARGET2).c
	gcc -I/opt/packages/papi/5.4.3/include/ -O0 $(TARGET2).c -L/opt/packages/papi/5.4.3/lib/ -lpapi -o $(TARGET2).out -D$(OPTS) -D N=$(SIZE)

$(TARGET3):$(TARGET3).c
	$(CC) -mkl -O0 $(TARGET3).c -o $(TARGET3).out -D mdim=$(SIZE) -D ndim=$(SIZE) -D kdim=$(SIZE)

$(TARGET4):$(TARGET4).c
	gcc -O0 $(TARGET4).c -o $(TARGET4).out -D N=$(SIZE)
clean:
	$(RM) $(TARGET1).out $(TARGET2).out $(TARGET3).out $(TARGET4).out
