obj-m += lab1.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod lab1.ko
	sudo dmesg -C
user:
	gcc -pedantic-errors -Wall -Werror -g3 -O0 --std=c99 -fsanitize=address,undefined,leak ./user.c -o user
clean:
	sudo rmmod lab1
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
