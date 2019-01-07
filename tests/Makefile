CXX ?= g++

SRC_PATH = ./
MAIN_DIR = ../src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

BIN_NAME = run_tests
SRC_EXT = cpp

# workaround for source files in different folders
# remove leading MAIN_DIR and adding it into VPATH to look for missing files
SOURCES = $(MAIN_DIR)/distributed_list_median.cpp \
	test_distributed_list_median.cpp
VPATH = $(MAIN_DIR)
OBJECTS = $(patsubst %.$(SRC_EXT),$(BUILD_PATH)/%.o,$(subst $(MAIN_DIR)/,,$(SOURCES)))

# flags #
COMPILE_FLAGS = -std=c++11 -Wall -Wextra -g -I $(MAIN_DIR)

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: %.$(SRC_EXT) Makefile
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@

# Debug
print-%  : ; @echo $* = $($*)
