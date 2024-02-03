# HTTP OS

This is a hobby operating system that acts as a http server.

Inspired by [Sami's 486 server](http://486servu.dy.fi/index_en.htm)

## What does this Operating System do?

This operating system runs a web server (that's planned to be written in Python) that serves websites to users.

### Why are you making this?

To quote the [OSDev wiki's introduction page](https://wiki.osdev.org/Introduction#Why_develop_an_OS.3F)

> Having complete control over the machine. When developing an application or other userspace program, the developer has to take the code written by others into consideration: the operating system, the libraries, other programs, etc. It is a powerful feeling for the only code to be running on a machine to be your own.

and most importantly

>Because it's fun. Low level programming is a fun and exciting task, because you have to do everything. This may seem more difficult (it is, don't worry) but by the same reasons more fun. You know how everything works, how it all fits, and the inner-most workings of your program.

## How do I build?

Install these packages:
- Debian (and Ubuntu, Mint, WSL, etc) users: `sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo qemu-utils qemu-system-x86 qemu-system-gui bochs`
- Arch Linux (and Manjaro) users: `sudo (your favorite AUR helper) -Sy base-devel gmp libmp3 mpfr qemu-desktop bochs`
- Windows users: Install WSL by following [this guide](https://learn.microsoft.com/en-us/windows/wsl/install) and follow Debian steps

After installing packages build a GCC Cross-Compiler by following [this guide](https://wiki.osdev.org/GCC_Cross-Compiler), the target should be `i686-elf` and the version of GCC and binutils used is `14.0.0` and `2.41.50.20230920` respectively.

Once done building the cross compiler you can go to the root of the project folder and run `make run` to run the operating system.
To copy to a floppy disk build the operating system via `make` and then run `./floppycopy` as root and have a floppy disk inserted.

## Resources
[Nanobyte's OS videos](https://www.youtube.com/playlist?list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN)
[OSDev Wiki](https://wiki.osdev.org/Main_Page)
[CPlusPlus.com](https://cplusplus.com/reference/) (For libc function references)