DEVICE     := attiny85
CLOCK      := 1000000
PROGRAMMER := stk500v1

# avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i 

TARGET_HEX ?= a.hex
TARGET_EXEC ?= a.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

PREFIX := avr-
CC := $(PREFIX)gcc
AS := $(PREFIX)gas
CXX := $(PREFIX)g++
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size
PGM := avrdude

SRCS := $(shell find $(SRC_DIRS) -name \*.cpp -or -name \*.c -or -name \*.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

$(BUILD_DIR)/$(TARGET_HEX): $(BUILD_DIR)/$(TARGET_EXEC) $(OBJS)
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	$(SIZE) --format=avr --mcu=$(DEVICE) $@

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
