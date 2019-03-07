#!/bin/sh
avr-nm -Crtd --size-sort ./build/launchpad.ino.elf | grep -i ' [dbv] '