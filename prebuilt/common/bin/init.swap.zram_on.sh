#! /vendor/bin/sh
mkswap /dev/block/zram0
echo 1 > /sys/module/zswap/parameters/enabled
swapon -p 100 /dev/block/zram0
