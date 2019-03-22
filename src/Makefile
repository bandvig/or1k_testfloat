.PHONY: all tfdf tfsf tfall clean help

OR32_TOOL_PREFIX=or1k-elf-
#OR32_TOOL_PREFIX=or1k-linux-uclibc-

OR32_LD=$(OR32_TOOL_PREFIX)ld
OR32_AS=$(OR32_TOOL_PREFIX)as
OR32_CC=$(OR32_TOOL_PREFIX)gcc
OR32_AR=$(OR32_TOOL_PREFIX)ar
OR32_RANLIB=$(OR32_TOOL_PREFIX)ranlib
OR32_OBJDUMP=$(OR32_TOOL_PREFIX)objdump
OR32_OBJCOPY=$(OR32_TOOL_PREFIX)objcopy

#UTILS_BIN2HEX=bin2hex
#UTILS_BIN2VMEM=bin2vmem


# Machine flags - uncomment one or create custom combination of flags
# All hardware flags
#MARCH_FLAGS ?=-mhard-mul -mhard-div -mhard-float -mdouble-float -mboard=atlys
# For MAROCCHINO:
MARCH_FLAGS ?=-mboard=atlys
# Hardware integer arith, soft float
#MARCH_FLAGS ?=-mhard-mul -mhard-div -msoft-float
# FPGA default - only hardware multiply
#MARCH_FLAGS ?=-mhard-mul -msoft-div -msoft-float
# All software div, mul and FPU
#MARCH_FLAGS ?=-msoft-mul -msoft-div -msoft-float

OR32_CFLAGS ?= -pipe -O0 $(MARCH_FLAGS)

common_testfloat_sources = test_float_fail.c \
random.c    \
writeHex.c  \
softfloat.c \
testCases.c \
testLoops.c

#testsoftfloat_SOURCES    = slowfloat.c \
#testsoftfloat.c

testfloat_SOURCES = systmodes.c \
systflags.c    \
systfloat.S    \
testFunction.c \
testfloat.c

TF_SOURCES = $(common_testfloat_sources) $(testfloat_SOURCES)

all: tfall

# Both Single and Double Precisions
tfall:
	rm -f ../bin/tfall.* ~/tftpshare/tfall.ubi
	$(OR32_CC) $(OR32_CFLAGS) -mhard-float -mdouble-float -DSYST_FLOAT32 -DSYST_FLOAT64 $(TF_SOURCES) -o ../bin/tfall.elf
	$(OR32_OBJDUMP) -d ../bin/tfall.elf > ../bin/tfall.dis
	$(OR32_OBJCOPY) -O binary ../bin/tfall.elf ../bin/tfall.bin
	$(OR32_OBJCOPY) -I binary -O verilog ../bin/tfall.bin ../bin/tfall.vmem
	mkimage.exe -A or1k -T standalone -C none -a 0 -e 0x100 -n TestFloat -d ../bin/tfall.bin ~/tftpshare/tfall.ubi

# Single Precision only
tfsf:
	rm -f ../bin/tfsf.* ~/tftpshare/tfsf.ubi
	$(OR32_CC) $(OR32_CFLAGS) -mhard-float -DSYST_FLOAT32 $(TF_SOURCES) -o ../bin/tfsf.elf
	$(OR32_OBJDUMP) -d ../bin/tfsf.elf > ../bin/tfsf.dis
	$(OR32_OBJCOPY) -O binary ../bin/tfsf.elf ../bin/tfsf.bin
	$(OR32_OBJCOPY) -I binary -O verilog ../bin/tfsf.bin ../bin/tfsf.vmem
	mkimage.exe -A or1k -T standalone -C none -a 0 -e 0x100 -n TestFloat -d ../bin/tfsf.bin ~/tftpshare/tfsf.ubi

# Double Precision only
tfdf:
	rm -f ../bin/tfdf.* ~/tftpshare/tfdf.ubi
	$(OR32_CC) $(OR32_CFLAGS) -mhard-float -mdouble-float -DSYST_FLOAT64 $(TF_SOURCES) -o ../bin/tfdf.elf
	$(OR32_OBJDUMP) -d ../bin/tfdf.elf > ../bin/tfdf.dis
	$(OR32_OBJCOPY) -O binary ../bin/tfdf.elf ../bin/tfdf.bin
	$(OR32_OBJCOPY) -I binary -O verilog ../bin/tfdf.bin ../bin/tfdf.vmem
	mkimage.exe -A or1k -T standalone -C none -a 0 -e 0x100 -n TestFloat -d ../bin/tfdf.bin ~/tftpshare/tfdf.ubi

# clean up
clean:
	rm -f ../bin/tfall.* ~/tftpshare/tfall.ubi
	rm -f ../bin/tfsf.* ~/tftpshare/tfsf.ubi
	rm -f ../bin/tfdf.* ~/tftpshare/tfdf.ubi

# Help
help:
	@echo "  Single and Double Precisions for Atlys board (default):"
	@echo "    $ make"
	@echo "  Single Precision only for Atlys board:"
	@echo "    $ make tfsf"
	@echo "  Double Precision nonly for Atlys board:"
	@echo "    $ make tfdf"
	@echo "  Clean up:"
	@echo "    $ make clean"
