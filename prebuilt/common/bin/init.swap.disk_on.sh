#! /vendor/bin/sh

mkdir /data/swap

if [ ! -f /data/swap/swapfile ]; then
    dd if=/dev/zero of=/data/swap/swapfile bs=1024 count=2097152
fi

mkswap /data/swap/swapfile
swapon -p 0 /data/swap/swapfile
