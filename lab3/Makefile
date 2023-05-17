obj-m += virt_net_if.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod virt_net_if.ko
clean:
	sudo rmmod virt_net_if
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
