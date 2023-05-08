CC := gcc
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin 

EXE := $(BIN_DIR)/store

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# C PreProcessor flags
CPPFLAGS := -Iinclude
CFLAGS := -Wall
LDFLAGS := 
LDLIBS :=            

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
    $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
    mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean

-include $(OBJ:.o=.d)

