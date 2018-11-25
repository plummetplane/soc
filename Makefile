# use cross-compiler
CROSS_COMPILE ?= arm-none-eabi-

# tools
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
LD := $(CROSS_COMPILE)gcc

# other important flags
CSTD ?= -std=c99
OPT ?= -Os
DEBUG ?= -ggdb3

# device setup
OPENCM3_LIB = opencm3_stm32f1
OPENCM3_DEFS = -DSTM32F1
ARCH_FLAGS = -mthumb -mcpu=cortex-m3
LDSCRIPT = stm32f103x8.ld

# compiler flags
CFLAGS += $(OPT) $(CSTD) $(DEBUG)
CFLAGS += $(ARCH_FLAGS) $(OPENCM3_DEFS)
CFLAGS += -fno-common
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wextra -Wshadow -Wno-unused-variable -Wimplicit-function-declaration
CFLAGS += -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes
CFLAGS += -Iinclude

# linker flags
LDFLAGS += $(OPT) $(DEBUG)
LDFLAGS += -T$(LDSCRIPT) -nostartfiles
LDFLAGS += $(ARCH_FLAGS)
LDFLAGS += --static
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--print-gc-sections

LDLIBS += -l$(OPENCM3_LIB)
LDLIBS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

# target objects
OBJ = main.o \
      ppm-decode/ppm-readval.o

#rules
all: main.elf

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

main.elf: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) *.elf
