#!/usr/bin/bash

dev1=/dev/loop92
dev2=/dev/loop93

echo "Creating hdd.img..."
dd if=/dev/zero of=./hdd.img bs=512 count=16065 1>/dev/null 2>&1

# dev1=$(losetup | grep hdd.img | cut -d" " -f1 | head -n 1)

losetup $dev1 ./hdd.img

echo "Creating bootable first FAT32 partition... $dev1"
(echo c; echo u; echo p; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk $dev1 || true

# cupnp1  a1tcw

echo "Mounting partition to $dev2..."
losetup $dev2 ./hdd.img \
--offset    `echo \`fdisk -lu $dev1 | sed -n 9p | awk '{print $3}'\`*512 | bc` \
--sizelimit `echo \`fdisk -lu $dev1 | sed -n 9p | awk '{print $4}'\`*512 | bc`
losetup -d $dev1  

echo "Format partition..."
echo $dev2
mkdosfs $dev2

# echo "Install grub on device..."
# grub-install --targe=i386-pc --recheck --debug --boot-directory=/boot $dev2

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
echo "device (hd0) hdd.img \n \
        root (hd0,0)         \n \
        setup (hd0)          \n \
        quit\n" | ./grub_bin --batch
echo "Done!"