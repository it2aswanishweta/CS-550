## Description

In this assignment, there are two user-level C-programs for a producer and consumer, where the producers and consumers run concurrently in two text terminals.
Here we observe the behavior of the producers and consumers if one gets killed or both run simultaneously.

Next thing is we do is to create our own character device, this device maintains its own FIFO queue of maximum N numbers.

-------------------------------------------------------------------------------------------------------------------------------
## Command to run the code

1) make clean
2) make
3) insmod chardev3.ko buffersize=(any size you want to give)
4) cat /proc/devices (from here you will get the major)
4) mknod /dev/chardev3 c MAJOR MINOR
5) ./myproducer /dev/chardev3
6) ./myconsumer /dev/chardev3
