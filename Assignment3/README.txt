1) make clean
2) make
3) insmod chardev3.ko buffersize=(any size you want to give)
4) cat /proc/devices (from here you will get the major)
4) mknod /dev/chardev3 c MAJOR MINOR
5) ./myproducer /dev/chardev3
6) ./myconsumer /dev/chardev3