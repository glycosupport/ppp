#!/usr/bin/bash

dev1=/dev/loop90
dev2=/dev/loop91

echo "Creating hdd.img..."
dd if=/dev/zero of=./hdd.img bs=512 count=16065 1>/dev/null 2>&1

losetup $dev1 ./hdd.img

echo "Creating bootable first FAT32 partition... $dev1"
(echo c; echo u; echo p; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk $dev1 || true

echo "Mounting partition to $dev2..."
losetup $dev2 ./hdd.img \
--offset    `echo \`fdisk -lu $dev1 | sed -n 9p | awk '{print $3}'\`*512 | bc` \
--sizelimit `echo \`fdisk -lu $dev1 | sed -n 9p | awk '{print $4}'\`*512 | bc`
losetup -d $dev1  

echo "Format partition..."
echo $dev2
mkdosfs $dev2

echo "Copy kernel and grub files on partition..."
mkdir -p tempdir
mount $dev2 tempdir
mkdir tempdir/boot
cp -r grub tempdir/boot/
cp kernel.bin tempdir/
sleep 1
umount $dev2
rm -r tempdir
losetup -d $dev2

echo "Installing GRUB..."
(echo "device (hd0) hdd.img"; echo "root (hd0,0)"; echo "setup (hd0)"; echo "quit") | ./grub_bin --batch
echo "Done!"