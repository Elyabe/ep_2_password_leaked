CXX=mpic++
CXXFLAGS=-O3
LDFLAGS=-O3

OBJECTS=main.o sha1.o

sha1: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o sha1 $(OBJECTS)

all:	sha1

run:	all
	mpirun -np 32 --oversubscribe ./sha1  902ba3cda1883801594b6e1b452790cc53948fda
# 	mpirun -np 32 --oversubscribe ./sha1  4051c87045623d87ca8f6982fc8b2c971474bcb4
# 	mpirun -np 32 --oversubscribe ./sha1  fc1aedf39e3245eb7aac3db3581ee786001f6479
# 	mpirun -np 8 --oversubscribe ./sha1  8cb2237d0679ca88db6464eac60da96345513964
# 	mpirun -np 4 --oversubscribe ./sha1  0776f15b676177c7c888da2d17e5f82a6e2a43d7


clean:
	rm -f *.optrpt *.o sha1 *~ 
