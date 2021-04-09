ROOT = /
TARGET = $(BIN_DIR)/main
SRC_DIR = ./src
INC_DIR = ./inc
OBJ_DIR = ./obj
BIN_DIR = ./bin
OBJS = $(OBJ_DIR)/main.o \
	   $(OBJ_DIR)/hashtable.o \
	   $(OBJ_DIR)/iniparse.o \
	   $(OBJ_DIR)/dbusfunctions.o
CFLAGS = -Wall -I$(INC_DIR) `pkg-config --cflags dbus-1` `pkg-config --libs dbus-1`

$(TARGET) : $(OBJS)
	@mkdir -p $(BIN_DIR)
	@gcc $(CFLAGS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@gcc -c -MD $(CFLAGS) $< -o $@

-include $(OBJ_DIR)*.d

.PHONY : all
all: $(TARGET)

.PHONY : clean
clean:
	@rm -r $(OBJ_DIR) $(BIN_DIR)

.PHONY : install
install:
	@mkdir $(ROOT)/etc/example
	@cp ./etc/daemon.conf $(ROOT)/etc/example

.PHONY : uninstall
uninstall:
	@rm -rfv $(ROOT)/etc/example
