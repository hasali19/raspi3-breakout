TC_PREFIX = aarch64-elf

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding \
		 -mcpu=cortex-a53 -Iinclude -DPRINTF_DISABLE_SUPPORT_FLOAT

LIB_DIR = $(shell dirname $$(which $(TC_PREFIX)-gcc))/../$(TC_PREFIX)/lib

CC  = $(TC_PREFIX)-gcc $(CFLAGS)
ASM = $(TC_PREFIX)-gcc
LD  = $(TC_PREFIX)-ld -L $(LIB_DIR)

SRC_DIR   = src
BUILD_DIR = build

.PHONY: all clean objdump

all: $(BUILD_DIR)/kernel8.img

clean:
	rm -rf $(BUILD_DIR)

objdump: $(BUILD_DIR)/kernel8.elf
	$(TC_PREFIX)-objdump -D $(BUILD_DIR)/kernel8.elf

# C files
$(BUILD_DIR)/obj/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -MMD -c $< -o $@

# Assembly files
$(BUILD_DIR)/obj/%.S.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(ASM) -MMD -c $< -o $@

# Find all .c and .S files in the src directory
# and determine the neccessary object files to create for
# each source file.
C_FILES   = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/obj/%.c.o) \
			$(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/obj/%.S.o)

# Include dependency files generated by gcc for
# each c file.
-include $(OBJ_FILES:%.o=%.d)

# Link object files into single binary.
$(BUILD_DIR)/kernel8.elf: link.ld $(OBJ_FILES)
	$(LD) -T link.ld $(OBJ_FILES) -lm -o $@

# Strip the elf file to produce a plain bare metal
# executable binary.
$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/kernel8.elf
	$(TC_PREFIX)-objcopy $< -O binary $@
