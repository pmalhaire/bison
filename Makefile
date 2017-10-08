sources = $(addprefix src/, main.cpp bison.cpp bison_objects.cpp)
headers = $(addprefix include/, bison.hpp bison_types.hpp bison_read_bytes.hpp bison_objects.hpp)

unit_tests = hello small id array time
sample_tests = mongodump

DEBUG ?= 0
PLAT = $(shell uname)

BUILD_DIR = build/$(PLAT)

ifeq ($(DEBUG), 1)
	G_FLAG = -g 
	BUILD_DIR += /debug
endif

L1 = =========================================
L2 = -----------------------------------------

.PHONY: clean debug bison test unit_test sample_test $(unit_tests) $(sample_tests)

bison: $(BUILD_DIR)/bison

$(BUILD_DIR)/bison: $(sources) $(headers)
	@mkdir -p $(BUILD_DIR)
	g++ -Wall -std=c++14 $(G_FLAG) -Iinclude $(sources) -o $(basename $@)

test: bison unit_test sample_test

unit_test: bison $(unit_tests)

sample_test: bison $(sample_tests)

$(unit_tests):
	@echo
	@echo $(L1)
	@echo "=     $@"
	@echo $(L1)
	@./$(BUILD_DIR)/bison $(addsuffix .bson, ./test/$@) #$(addsuffix .json, ./test/$@)
	@echo $(L2)


$(sample_tests):
	@echo
	@echo $(L1)
	@echo "=     $@"
	@echo $(L1)
	@./$(BUILD_DIR)/bison $(addsuffix .bson, ./test/$@) #$(addsuffix .json, ./test/$@)
	@echo $(L2)


clean:
	rm -rf $(BUILD_DIR)
