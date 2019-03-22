# or1k_testfloat
FPU verification tool for OpenRISC based on softfloat library
# Attention
 1) GCC 5.4.0 NewLIB based OR1K-toolchain `or1k-elf-*` is required. Default board is `Atlys` (edit `Makefile` to change it).
 2) All of printf()'s format stings were edited to replace Unix-style line endings by Window-style ones
 3) Commands in `Makefile` manipulate with U-boot image files assumed to be placed into `home/tftpshare`. Also `mkimage.exe` is pre-reqiured to generate them. Perhaps, it would be good idea to remove / comment all such references before run `make`.
