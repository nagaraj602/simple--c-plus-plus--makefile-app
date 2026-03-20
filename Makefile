# Variables
SRC_DIR=src
BUILD_DIR=build
TARGET=app
PORT=8087

# Default target
all: run

# Compile
build:
	mkdir -p $(BUILD_DIR)
	g++ $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/$(TARGET)

# Run
run: build
	@echo "🚀 Starting application..."
	@echo "👉 You can access the application at: http://$$(curl -s ifconfig.me):$(PORT)"
	./$(BUILD_DIR)/$(TARGET)

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Show URL
url:
	@echo "http://$$(curl -s ifconfig.me):$(PORT)"
