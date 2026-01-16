BUILD_DIR = build
EXEC_NAME = LinAim

all: build

configure:
	cmake -S . -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR)

run: build
	@echo "------- Executando $(EXEC_NAME) ---------"
	./$(BUILD_DIR)/LinAim

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all configure build run clean