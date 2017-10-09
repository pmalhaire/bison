sources = $(addprefix src/, main.cpp bison.cpp bison_objects.cpp)
headers = $(addprefix include/, bison.hpp bison_types.hpp bison_read_bytes.hpp bison_objects.hpp)

unit_tests = hello small array time ints #id is not working yet
sample_tests = mongodump

DEBUG ?= 0
PLAT = $(shell uname)

BUILD_DIR = build/$(PLAT)

ifeq ($(DEBUG), 1)
	G_FLAG = -g 
	BUILD_DIR += /debug
endif

UNIT_TEST_DIR = $(BUILD_DIR)/unit_tests

L1 = =========================================
L2 = -----------------------------------------
L3 = !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

.PHONY: clean debug bison test unit_test sample_test $(unit_tests) $(sample_tests)

bison: $(BUILD_DIR)/bison

$(BUILD_DIR)/bison: $(sources) $(headers)
	@mkdir -p $(BUILD_DIR)
	@g++ -Wall -std=c++14 $(G_FLAG) -Iinclude $(sources) -o $(basename $@)
	@echo bison build OK

test: bison unit_test sample_test

unit_test: bison $(unit_tests)

sample_test: bison $(sample_tests)

$(unit_tests):
	@rm -rf $(UNIT_TEST_DIR)
	@mkdir -p $(UNIT_TEST_DIR)
	@./$(BUILD_DIR)/bison $(addsuffix .bson, ./test/$@) > $(UNIT_TEST_DIR)/$@
	@tr -d '\n' < $(UNIT_TEST_DIR)/$@ > $(UNIT_TEST_DIR)/$(addsuffix .json,$@) #removing the \n not elegant but working
	@rm $(UNIT_TEST_DIR)/$@ #removing the temporary file
	@echo >> $(UNIT_TEST_DIR)/$(addsuffix .json,$@) #adding back the end \n 
	@diff -w $(addsuffix .json, ./test/$@) \
	$(UNIT_TEST_DIR)/$(addsuffix .json,$@) || ( echo $(L3) && echo test $@ KO && false )
	@rm $(UNIT_TEST_DIR)/$(addsuffix .json,$@)
	@echo "=     $@ OK"


$(sample_tests):
	@echo
	@echo $(L1)
	@echo "=     $@"
	@echo $(L1)
	@./$(BUILD_DIR)/bison $(addsuffix .bson, ./test/$@)
	@echo $(L2)
	@echo "=     $@ OK"


clean:
	rm -rf $(BUILD_DIR)
