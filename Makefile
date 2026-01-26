BUILD_DIR = build
EXEC_NAME = LinAim
EXEC_PATH = $(BUILD_DIR)/$(EXEC_NAME)

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

build: configure
	cmake --build $(BUILD_DIR)

run: build
	@echo "------- Executando $(EXEC_NAME) ---------"
	./$(BUILD_DIR)/LinAim

clean:
	rm -rf $(BUILD_DIR)
	rm -f valgrind-out.txt

debug:
	@echo "Configurando modo DEBUG..."
	rm -rf $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR) -j$(NPROCS)

asan: 
	rm -rf $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_FLAGS="-fsanitize=address -g -fno-omit-frame-pointer" \
		-DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"
	cmake --build $(BUILD_DIR) -j$(NPROCS)
	LSAN_OPTIONS=suppressions=suppr.txt ./$(EXEC_PATH)


valgrind: clean

	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

	cmake --build $(BUILD_DIR)

	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--log-file=valgrind-out.txt \
		./$(BUILD_DIR)/$(EXEC_NAME)
	
	@echo "---------------------------------------------------"
	@echo "Teste concluído. Verifica o ficheiro 'valgrind-out.txt'"
	@echo "---------------------------------------------------"

.PHONY: all configure build run clean debug asan valgrind
