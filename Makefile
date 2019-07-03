CXX=mpic++
CXXFLAGS=-O3
LDFLAGS=-O3

OBJECTS=main.o sha1.o

sha1: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o sha1 $(OBJECTS)

all:	sha1

run:	all
# 	mpirun -np 8 --oversubscribe ./sha1  902ba3cda1883801594b6e1b452790cc53948fda
# 	mpirun -np 8 --oversubscribe ./sha1  4051c87045623d87ca8f6982fc8b2c971474bcb4
	mpirun -np 8 --oversubscribe ./sha1  8cb2237d0679ca88db6464eac60da96345513964


clean:
	rm -f *.optrpt *.o sha1 *~ 
