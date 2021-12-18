.PHONY: cmake default check clean distclean install

BUILD_DIR := build

CMAKE := cmake

CMAKE_FLAGS :=

$(BUILD_DIR): CMakeLists.txt
	$(CMAKE) $(CMAKE_FLAGS) -S. -H. -B$(BUILD_DIR)
cmake: $(BUILD_DIR)

default: $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --verbose
check: default $(BUILD_DIR)
	./dist/Debug/test
clean distclean install: %: $(BUILD_DIR)
	$(CMAKE) --build $(BUILD_DIR) --target $@

.DEFAULT_GOAL = default
