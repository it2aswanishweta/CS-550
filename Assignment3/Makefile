obj-m = chardev3.o

KBUILD_CFLAGS += -w

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules $(KBUILD_CFLAGS)
	gcc myproducer.c -o myproducer.o
	gcc myconsumer.c -o myconsumer.o
	gcc -o myproducer myproducer.c
	gcc -o myconsumer myconsumer.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
