# Compiler
CXX = g++

# Directories
SRC_DIR = src
BIN_DIR = bin
DEBUG_DIR = debug
OUTPUT_DIR = outputfiles
INPUT_DIR = inputfiles/created_test_cases

# Targets
all: $(BIN_DIR)/forward $(BIN_DIR)/noforward

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	mkdir -p $(DEBUG_DIR)
	mkdir -p $(OUTPUT_DIR)
	chmod 777 $(DEBUG_DIR)
	chmod 777 $(OUTPUT_DIR)

# Forwarding executable
$(BIN_DIR)/forward: $(SRC_DIR)/forwarding_main.cpp | $(BIN_DIR)
	$(CXX) $(SRC_DIR)/forwarding_main.cpp -o $(BIN_DIR)/forward
	chmod 777 $(BIN_DIR)/forward

# Non-forwarding executable
$(BIN_DIR)/noforward: $(SRC_DIR)/non_forwarding_main.cpp | $(BIN_DIR)
	$(CXX) $(SRC_DIR)/non_forwarding_main.cpp -o $(BIN_DIR)/noforward
	chmod 777 $(BIN_DIR)/noforward

# Run executables
run:
	@if [ -z "$(TARGET)" ] || [ -z "$(ARGS)" ]; then \
		echo "Usage: make run TARGET=<forward|noforward> ARGS=<args>"; \
	else \
		for file in $(INPUT_DIR)/*; do \
			echo "Running ./$(BIN_DIR)/$(TARGET) $$file $(ARGS)"; \
			./$(BIN_DIR)/$(TARGET) $$file $(ARGS); \
		done; \
	fi

# Clean up
clean:
	rm -rf $(BIN_DIR)
	rm -rf $(DEBUG_DIR)
	rm -rf $(OUTPUT_DIR)

# Phony targets
.PHONY: all clean run
