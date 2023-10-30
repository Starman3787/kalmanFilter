TARGET = main

# Define the linker script location and chip architecture
LD_SCRIPT	= link/stm32f412zgtx.ld
MCU_SPEC	= cortex-m4

# Toolchain definitions (ARM bare metal defaults)
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

# Assembly directives
ASFLAGS += -c
ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -g3
CFLAGS += -O0
CFLAGS += -std=c17
CFLAGS += -D_FORTIFY_SOURCE=2
CFLAGS += -DSYSTEM_DEBUG__
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nosys.specs
CFLAGS += -DSTM32H7A3xxQ
CFLAGS += -Wno-main
CFLAGS += -Wno-sequence-point
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-variable
CFLAGS += -fcommon

# Linker directives.
LSCRIPT = ./$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += -Wextra
LFLAGS += --specs=nosys.specs
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)
LFLAGS += -Wl,--no-warn-rwx-segment

AS_SRC = ./src/startup.S
C_SRC = ./src/main.c
C_SRC += ./src/init.c
C_SRC += ./src/drivers/gtu7/gtu7.c
C_SRC += ./src/drivers/mpu6050/mpu6050.c

INCLUDE = -I.

OBJS = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf 
	rm -f $(TARGET).bin 