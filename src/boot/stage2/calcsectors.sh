#!/bin/bash
echo -n STAGE2_SIZE equ $(expr $(stat -c "%s" "./stage2.bin") / 512 + 1)