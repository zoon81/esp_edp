# Makefile for ESP8266 projects
#
# Thanks to:
# - zarya
# - Jeroen Domburg (Sprite_tm)
# - Christian Klippel (mamalala)
# - Tommie Gannert (tommie)
#
# Changelog:
# - 2014-10-06: Changed the variables to include the header file directory
# - 2014-10-06: Added global var for the Xtensa tool root
# - 2014-11-23: Updated for SDK 0.9.3
# - 2014-12-25: Replaced esptool by esptool.py

# Output directors to store intermediate compiled files
# relative to the project directory
BUILD_BASE	= build
FW_BASE		= firmware
HOME_FOLDER = $(HOME)

# base directory for the compiler
#XTENSA_TOOLS_ROOT ?= /home/t400/ESP8266/esp-open-sdk/xtensa-lx106-elf/bin/
XTENSA_TOOLS_ROOT ?= $(HOME_FOLDER)/ESP8266/esp-open-sdk/xtensa-lx106-elf/bin/

# esptool.py path and port
ESPTOOL		?= esptool.py

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	ESPPORT		?= /dev/ttyUSB0
	ESPBAUD		?= 230400
	# base directory of the ESP8266 SDK package, absolute
	SDK_BASE	?= $(HOME_FOLDER)/ESP8266/esp-open-sdk/ESP8266_NONOS_SDK_V2.0.0_16_08_10
endif
ifeq ($(UNAME_S),Darwin)
	ESPPORT		?= /dev/tty.wchusbserial410
	ESPBAUD		?= 115200
	# base directory of the ESP8266 SDK package, absolute
	SDK_BASE	?= $(HOME_FOLDER)/ESP8266/esp-open-sdk/ESP8266_NONOS_SDK/
endif



# name for the target project
TARGET		= blinky

# which modules (subdirectories) of the project to include in compiling
MODULES		= user user/HAL user/driver user/fonts user/widgets user/utils
EXTRA_INCDIR    = include

# libraries used in this project, mainly provided by the SDK
LIBS		= c gcc hal pp phy net80211 lwip wpa main ssl json

# compiler flags using during compilation of source files
CFLAGS		= -O2 -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH

# linker flags used to generate the main object file
LDFLAGS		= -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static

# linker script used for the above linkier step
LD_SCRIPT	= eagle.app.v6.ld

# various paths from the SDK used in this project
SDK_LIBDIR	= lib
SDK_LDDIR	= ld
SDK_INCDIR	= include include/json

# we create two different files for uploading into the flash
# these are the names and options to generate them
FW_FILE_1_ADDR	= 0x00000
FW_FILE_2_ADDR	= 0x10000

# select which tools to use as compiler, librarian and linker
CC		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
AS 		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-as
AR		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-ar
LD		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc

#General FileSystem stuff
FS_BASE_DIR = external_tools/spiffs_image
MKSPIFFS_TOOL = mkspiffs
WHOLE_SIZE = 65536
BLOCK_SIZE = 4096
PAGE_SIZE = 256
FS_BINARY = external_tools/spiffs_image.bin

# ESP FileSystem stuff
FS_BASE_ADDRESS = 0x63000

ENABLE_GDB := 0

#DEBUGING
ifeq ($(ENABLE_GDB), 1)
	CFLAGS += -Og -ggdb -DGDBSTUB_FREERTOS=0 -DENABLE_GDB=1
	MODULES		 += esp-gdbstub
	EXTRA_INCDIR += esp-gdbstub
else
	CFLAGS += -Os -g
endif

####
#### no user configurable options below here
####
SRC_DIR		:= $(MODULES)
BUILD_DIR	:= $(addprefix $(BUILD_BASE)/,$(MODULES))

SDK_LIBDIR	:= $(addprefix $(SDK_BASE)/,$(SDK_LIBDIR))
SDK_INCDIR	:= $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))

AS_SRC		:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.S)) 
C_SRC		:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
AS_OBJ		:= $(patsubst %.S,%.o,$(AS_SRC))
C_OBJ		:= $(patsubst %.c,%.o,$(C_SRC))
OBJ			:= $(patsubst %.o,$(BUILD_BASE)/%.o,$(AS_OBJ) $(C_OBJ))
LIBS		:= $(addprefix -l,$(LIBS))
APP_AR		:= $(addprefix $(BUILD_BASE)/,$(TARGET)_app.a)
TARGET_OUT	:= $(addprefix $(BUILD_BASE)/,$(TARGET).out)

LD_SCRIPT	:= $(addprefix -T$(SDK_BASE)/$(SDK_LDDIR)/,$(LD_SCRIPT))
#LD_SCRIPT := $(addprefix -Texternal_tools/,$(LD_SCRIPT))

INCDIR	:= $(addprefix -I,$(SRC_DIR))
EXTRA_INCDIR	:= $(addprefix -I,$(EXTRA_INCDIR))
MODULE_INCDIR	:= $(addsuffix /include,$(INCDIR))

FW_FILE_1	:= $(addprefix $(FW_BASE)/,$(FW_FILE_1_ADDR).bin)
FW_FILE_2	:= $(addprefix $(FW_BASE)/,$(FW_FILE_2_ADDR).bin)
VERBOSE := 0

V ?= $(VERBOSE)
ifeq ("$(V)","1")
Q :=
vecho := @true
else
Q := @
vecho := @echo
endif

vpath %.S $(SRC_DIR)
vpath %.c $(SRC_DIR)

define compile-objects
$1/%.o: %.S
	$(vecho) "ASM $$<"
	$(Q) $(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS) -D__ASSEMBLER__ -c $$< -o $$@

$1/%.o: %.c
	$(vecho) "CC $$<"
	$(Q) $(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS) -c $$< -o $$@
endef

.PHONY: all checkdirs flash clean

all: clean checkdirs $(TARGET_OUT) $(FW_FILE_1) $(FW_FILE_2)

$(FW_BASE)/%.bin: $(TARGET_OUT) | $(FW_BASE)
	$(vecho) "FW $(FW_BASE)/"
	$(Q) $(ESPTOOL) elf2image -o $(FW_BASE)/ $(TARGET_OUT)

$(TARGET_OUT): $(APP_AR)
	$(vecho) "LD $@"
	$(Q) $(LD) -L$(SDK_LIBDIR) $(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $(APP_AR) -Wl,--end-group -o $@

$(APP_AR): $(OBJ)
	$(vecho) "AR $@"
	$(Q) $(AR) cru $@ $^

checkdirs: $(BUILD_DIR) $(FW_BASE)

$(BUILD_DIR):
	$(Q) mkdir -p $@

$(FW_BASE):
	$(Q) mkdir -p $@

flash: $(FW_FILE_1) $(FW_FILE_2)
	$(ESPTOOL) --port $(ESPPORT) -b $(ESPBAUD) write_flash $(FW_FILE_1_ADDR) $(FW_FILE_1) $(FW_FILE_2_ADDR) $(FW_FILE_2)

fs_flash:
	$(MKSPIFFS_TOOL) -c $(FS_BASE_DIR) -b $(BLOCK_SIZE) -p $(PAGE_SIZE) $(FS_BINARY)
	$(ESPTOOL) --port $(ESPPORT) -b $(ESPBAUD) write_flash $(FS_BASE_ADDRESS) $(FS_BINARY)
fs_read:
	$(ESPTOOL) --port $(ESPPORT) -b $(ESPBAUD) read_flash $(FS_BASE_ADDRESS) $(WHOLE_SIZE) $(FS_BINARY)

clean:
	$(Q) rm -rf $(FW_BASE) $(BUILD_BASE)

$(foreach bdir,$(BUILD_DIR),$(eval $(call compile-objects,$(bdir))))
