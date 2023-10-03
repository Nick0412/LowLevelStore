#############################################################################
############################# Generic Functions #############################
#############################################################################
RECURSE = $(foreach dir,$(wildcard $1*),$(call RECURSE,$(dir)/,$2)$(filter $(subst *,%,$2),$(dir)))


#############################################################################
####################### Compiler and Linker Variables #######################
#############################################################################
# C Pre Processor flags
CPPFLAGS 	:= -Iinclude
CFLAGS   	:= -Wall -Wextra -Werror -std=c11
LDFLAGS  	:=
LDLIBS   	:= -lm


#############################################################################
######################### File and Folder Variables #########################
#############################################################################
SRC_DIR							:= src
OBJ_DIR							:= obj
BIN_DIR							:= bin
TEST_DIR						:= test

MAIN_PROGRAMS_DIR				:= $(SRC_DIR)/main-programs
MAIN_PROGRAMS_SRC_LIST			:= $(wildcard $(MAIN_PROGRAMS_DIR)/*.c)
MAIN_PROGRAMS_NO_PREFIX			:= $(patsubst $(SRC_DIR)/%,%,$(basename $(MAIN_PROGRAMS_SRC_LIST)))
MAIN_PROGRAMS_OBJ_LIST			:= $(patsubst $(MAIN_PROGRAMS_DIR)/%,$(OBJ_DIR)/main-programs/%,$(patsubst %.c,%.o,$(MAIN_PROGRAMS_SRC_LIST)))
MAIN_PROGRAMS_EXE_LIST			:= $(patsubst $(MAIN_PROGRAMS_DIR)/%,$(BIN_DIR)/main-programs/%,$(patsubst %.c,%,$(MAIN_PROGRAMS_SRC_LIST)))

SOURCES							:= $(filter-out $(MAIN_PROGRAMS_DIR)/%,$(call RECURSE,$(SRC_DIR),*.c))
OBJECTS							:= $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(patsubst %.c,%.o,$(SOURCES)))
SOURCE_NO_PREFIX_OR_EXTENSION 	:= $(patsubst $(SRC_DIR)/%,%,$(basename $(SOURCES)))

TEST_OBJECT_DIR					:= $(OBJ_DIR)/$(TEST_DIR)
TEST_BIN_DIR					:= $(BIN_DIR)/$(TEST_DIR)
TEST_SOURCES					:= $(call RECURSE,$(TEST_DIR),*.c)
TEST_OBJECTS					:= $(patsubst $(TEST_DIR)/%,$(OBJ_DIR)/$(TEST_DIR)/%,$(patsubst %.c,%.o,$(TEST_SOURCES)))
TEST_EXES						:= $(patsubst $(TEST_DIR)/%,$(TEST_BIN_DIR)/%,$(patsubst %.c,%,$(TEST_SOURCES)))
TEST_FILES_CLEAN				:= $(basename $(patsubst test/%,%,$(TEST_SOURCES)))


#############################################################################
############################## Setup Functions ##############################
#############################################################################
# Create nested directories for object files
$(shell mkdir -p $(sort $(dir $(OBJECTS))))

# Create nested directories for main object files and main executables
$(shell mkdir -p $(OBJ_DIR)/main-programs)
$(shell mkdir -p $(BIN_DIR)/main-programs)

# Create nested directories for test object files and test executables
$(shell mkdir -p $(OBJ_DIR)/test)
$(shell mkdir -p $(sort $(dir $(TEST_OBJECTS))))
$(shell mkdir -p $(BIN_DIR)/test)
$(shell mkdir -p $(sort $(dir $(TEST_EXES))))


#############################################################################
########################## Target Recipe Templates ##########################
#############################################################################
# 1. The prefixed directory file: folderA/some-file
define SOURCE_COMPILE_TEMPLATE
$$(OBJ_DIR)/$(1).o: $$(SRC_DIR)/$(1).c | $$(OBJ_DIR)
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -c $$< -o $$@
endef

# 1. Executable name: main1
# 2. The object name with prefix directory: main-programs/main1
define EXE_BUILD_TEMPLATE
$$(BIN_DIR)/$(1): $$(OBJ_DIR)/$(2).o $$(OBJECTS) | $$(BIN_DIR)
	$$(CC) $$(LDFLAGS) $$^ $$(LDLIBS) -o $$@
endef

define TEST_COMPILE_TEMPLATE
$$(TEST_OBJECT_DIR)/$(1).o: $$(TEST_DIR)/$(2).c | $$(TEST_OBJECT_DIR)
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -c $$< -o $$@
endef

define TEST_EXE_TEMPLATE
$$(TEST_BIN_DIR)/$(1): $$(TEST_OBJECT_DIR)/$(2).o $$(OBJECTS) | $$(TEST_BIN_DIR)
	$$(CC) $$(LDFLAGS) $$^ $$(LDLIBS) -o $$@
endef

#############################################################################
######################## Recipe Template Evaluations ########################
#############################################################################
# MAIN_PROGRAMS_NO_PREFIX looks like: [main-programs/client main-programs/server]
$(foreach source_file,$(SOURCE_NO_PREFIX_OR_EXTENSION),$(eval $(call SOURCE_COMPILE_TEMPLATE,$(source_file))))
$(foreach main_file,$(MAIN_PROGRAMS_NO_PREFIX),$(eval $(call SOURCE_COMPILE_TEMPLATE,$(main_file))))
$(foreach main_program,$(MAIN_PROGRAMS_NO_PREFIX),$(eval $(call EXE_BUILD_TEMPLATE,$(main_program),$(main_program))))
$(foreach test_file,$(TEST_FILES_CLEAN),$(eval $(call TEST_COMPILE_TEMPLATE,$(test_file),$(test_file))))
$(foreach test_file,$(TEST_FILES_CLEAN),$(eval $(call TEST_EXE_TEMPLATE,$(test_file),$(test_file))))


#############################################################################
############################ Real Target Recipes ############################
#############################################################################
.PHONY: clean all objects main-objects debug test

all: $(MAIN_PROGRAMS_EXE_LIST)

test: $(TEST_EXES)
	$(info ************  RUNNING TESTS ************)
	@for test in $(TEST_EXES); do \
		./$$test; \
	done

objects: $(OBJECTS)

main-objects: $(MAIN_PROGRAMS_OBJ_LIST)

$(OBJ_DIR) $(BIN_DIR) $(TEST_OBJECT_DIR) $(TEST_BIN_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
	@$(RM) -rf $(BIN_DIR) $(OBJ_DIR)

# 1. Get list of main programs (src/main-programs/sample1.c)
# 2. For each main program, compile it and place it in the object directory (obj/main-programs/sample1.o)
# 3. Link each main program object file against others and output executable (bin/main-programs/sample1)
debug:
	$(info $$MAIN_PROGRAMS_SRC_LIST is [${MAIN_PROGRAMS_SRC_LIST}])
	$(info $$MAIN_PROGRAMS_NO_PREFIX is [${MAIN_PROGRAMS_NO_PREFIX}])
	$(info $$MAIN_PROGRAMS_OBJ_LIST is [${MAIN_PROGRAMS_OBJ_LIST}])
	$(info $$MAIN_PROGRAMS_EXE_LIST is [${MAIN_PROGRAMS_EXE_LIST}])
	$(info $$SOURCES is [${SOURCES}])
	$(info $$OBJECTS is [${OBJECTS}])
	$(info $$BASES is [$(basename $(SOURCE_NO_PREFIX_OR_EXTENSION))])
	$(info $$DIRECTORIES is [$(sort $(dir $(OBJECTS)))])
	$(info $$TEST_SOURCES is [$(TEST_SOURCES)])
	$(info $$TEST_OBJECTS is [$(TEST_OBJECTS)])
	$(info $$TEST_EXES is [$(TEST_EXES)])
	$(info $$TEST_FILES_CLEAN is [$(TEST_FILES_CLEAN)])
