sources = $(addprefix src/, bison.cpp bison_objects.cpp)
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
BIG_TEST_DIR = $(BUILD_DIR)/big_test

L1 = =========================================
L2 = -----------------------------------------
L3 = !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

.PHONY: clean debug bison test unit_test sample_test $(unit_tests) $(sample_tests) big_test

bison: $(BUILD_DIR)/bison

$(BUILD_DIR)/bison: $(sources) $(headers) src/main.cpp
	@mkdir -p $(BUILD_DIR)
	@g++ -Wall -std=c++14 $(G_FLAG) -Iinclude $(sources) src/main.cpp -o $(basename $@)
	@echo bison build OK

test: bison sample_test unit_test

unit_test: bison $(unit_tests) acessors_tests


sample_test: bison $(sample_tests)
	@echo $(L2)

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

acessors_tests: $(sources) $(headers) src/accessors_tests.cpp
	@echo $(L2)
	@mkdir -p $(UNIT_TEST_DIR)
	@g++ -Wall -std=c++14 $(G_FLAG) -Iinclude $(sources) src/accessors_tests.cpp -o $(basename $(UNIT_TEST_DIR)/$@)
	@./$(UNIT_TEST_DIR)/$@


$(sample_tests):
	@echo
	@echo $(L1)
	@echo "=     $@"
	@echo $(L1)
	@./$(BUILD_DIR)/bison $(addsuffix .bson, ./test/$@)
	@echo $(L2)
	@echo "=     $@ OK"

big_test: bison
	@mkdir -p $(BIG_TEST_DIR)
	tar jxvf test/people.tar.bz2  -C $(BIG_TEST_DIR)
	@./$(BUILD_DIR)/bison $(BIG_TEST_DIR)/people.bson

clean:
	rm -rf $(BUILD_DIR)
