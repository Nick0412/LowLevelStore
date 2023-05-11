 # Helper function to perform recursive search in a directory for file types.
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

SRC_DIR 		:= src
BIN_DIR 		:= bin
OBJECT_DIR 		:= obj
# Outputs, both libraries and executables
EXE := $(BIN_DIR)/store
# Contains a list of all source files (*.c) with their nested directory structure. (example: src/dir1/file.c)
SOURCES 	:= $(call rwildcard,$(SRC_DIR),*.c)
# Contains the list of object files (*.o) and strips off directory prefix. (example: obj/file.o)
OBJECTS 	:= $(patsubst %.c,$(OBJECT_DIR)/%.o,$(notdir $(SOURCES)))
# Update VPATH to include all nested directories in src folder.
# Reference: https://www.gnu.org/software/make/manual/html_node/General-Search.html
VPATH 		:= $(sort $(dir $(SOURCES)))

# C Pre Processor flags
CPPFLAGS 	:= -Iinclude
CFLAGS   	:= -Wall
LDFLAGS  	:=
LDLIBS   	:=

.PHONY: all clean new test new2

all: $(EXE)

$(EXE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJECT_DIR)/%.o: %.c | $(OBJECT_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJECT_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJECT_DIR)

# ======================================================================== #
# ============================= Tests Section ============================ #
# ======================================================================== #

TEST_DIR			:= test
BIN_TEST_DIR		:= bin/test
OBJ_TEST_DIR		:= obj/test

TEST_SOURCES		:= $(strip $(call rwildcard,$(TEST_DIR),*.c))
PURE_TEST_SOURCES	:= $(basename $(notdir $(TEST_SOURCES)))
TEST_OBJECTS		:= $(patsubst %.c,$(OBJECT_DIR)/%.o,$(notdir $(TEST_SOURCES)))
TEST_EXES			:= $(patsubst %.c,$(BIN_TEST_DIR)/%,$(notdir $(TEST_SOURCES)))

OBJECTS_WITHOUT_MAIN := $(filter-out obj/main.o,$(OBJECTS))

define TEST_OBJECT_TEMPLATE
$$(OBJ_TEST_DIR)/$(1).o: $(2) | $$(OBJ_TEST_DIR)
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -c $$< -o $$@

$$(BIN_TEST_DIR)/$(1): $$(OBJ_TEST_DIR)/$(1).o $$(OBJECTS_WITHOUT_MAIN) | $$(BIN_TEST_DIR)
	$$(CC) $$(LDFLAGS) $$^ $$(LDLIBS) -o $$@
	
endef

$(foreach test_file,$(TEST_SOURCES),$(eval $(call TEST_OBJECT_TEMPLATE,$(basename $(notdir $(test_file))),$(test_file))))

$(BIN_TEST_DIR) $(OBJ_TEST_DIR):
	mkdir -p $@

test: $(TEST_EXES) all
	$(info ************  RUNNING TESTS ************)
	@for test in $(TEST_EXES); do \
		./$$test; \
	done

# Helper target to print out variable definitions.
new:
	$(info $$SOURCES is [${SOURCES}])
	$(info $$OBJECTS is [${OBJECTS}])
	$(info $$TEST_SOURCES is [${TEST_SOURCES}])
	$(info $$TEST_OBJECTS is [${TEST_OBJECTS}])
	$(info $$TEST_EXES is [${TEST_EXES}])
	$(info $$PURE_TEST_SOURCES is [${PURE_TEST_SOURCES}])
	$(foreach test_file,$(TEST_SOURCES),$(info $(call TEST_OBJECT_TEMPLATE,$(basename $(notdir $(test_file))),$(test_file))))
