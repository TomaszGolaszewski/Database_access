all: module.so
	gcc main.c -ldl -o target_bin.o
module.so: module.c module.h
	gcc -I . -shared -fPIC module.c -o module.so
clean:
	rm -rf *.o *.so

