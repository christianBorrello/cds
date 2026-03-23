CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c99 -g
INCLUDE = -Iinclude

SRC_DIR   = src
TEST_DIR  = tests
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target: build and run tests
all: compile_commands.json test

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Build the test binary
$(BUILD_DIR)/main: $(OBJS) $(TEST_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) $(TEST_DIR)/main.c -o $@

# Generate compile_commands.json for clangd
compile_commands.json: $(SRCS)
	@echo "[" > $@
	@first=1; for f in $(SRCS); do \
		if [ $$first -eq 0 ]; then echo "  ," >> $@; fi; \
		echo "  {" >> $@; \
		echo "    \"directory\": \"$$(pwd)\"," >> $@; \
		echo "    \"command\": \"$(CC) $(CFLAGS) $(INCLUDE) -c $$f -o $(BUILD_DIR)/$$(basename $$f .c).o\"," >> $@; \
		echo "    \"file\": \"$$f\"" >> $@; \
		echo "  }" >> $@; \
		first=0; \
	done
	@echo "]" >> $@

# Run it
test: $(BUILD_DIR)/main
	./$(BUILD_DIR)/main

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean
