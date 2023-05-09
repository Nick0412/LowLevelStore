# Helper function to perform recursive search in a directory for file types.
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC_DIR 	:= src
BIN_DIR 	:= bin
OBJECT_DIR 	:= obj
# Contains a list of all source files (*.c) with their nested directory structure. (example: src/dir1/file.c)
SOURCES 	:= $(call rwildcard,$(SRC_DIR),*.c)
# Contains the list of object files (*.o) and strips off directory prefix. (example: obj/file.o)
OBJECTS 	:= $(patsubst %.c,$(OBJECT_DIR)/%.o,$(notdir $(SOURCES)))
# Update VPATH to include all nested directories in src folder.
VPATH 		:= $(sort $(dir $(SOURCES)))

# Outputs, both libraries and executables
EXE 		:= $(BIN_DIR)/store

# C Pre Processor flags
CPPFLAGS := -Iinclude
CFLAGS   := -Wall
LDFLAGS  :=
LDLIBS   :=

.PHONY: all clean new

all: $(EXE)

$(EXE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJECT_DIR)/%.o: %.c | $(OBJECT_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJECT_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJECT_DIR)

# Helper target to print out variable definitions.
new:
	$(info $$SOURCES is [${SOURCES}])
	$(info $$OBJECTS is [${OBJECTS}])


-include $(OBJ:.o=.d)