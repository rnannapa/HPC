CC=gcc
N=N
SIZE=25000
OPTS=L
TARGET1=quicksortPAPI
TARGET2=quicksortPower
all:$(TARGET1) $(TARGET2)
$(TARGET1):$(TARGET1).c
	$(CC) -I/opt/packages/papi/5.4.3/include/ $(TARGET1).c -L/opt/packages/papi/5.4.3/lib/ -lpapi -o $(TARGET1) -D $(N)=$(SIZE) -D$(OPTS)

$(TARGET2):$(TARGET2).c
	$(CC) $(TARGET2).c -o $(TARGET2) -D $(N)=$(SIZE)
clean:
	$(RM) $(TARGET1) $(TARGET2)
