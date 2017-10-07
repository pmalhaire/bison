unit_tests = hello small id array

sample_tests = mongodump

L = =====================================

.PHONY: clean bison test unit_test sample_test $(unit_tests) $(sample_tests)

bison: build/bison

build/bison: src/main.cpp src/bison.cpp src/bison_types.cpp include/bison.hpp include/bison_types.hpp
	mkdir -p build
	g++ -Wall -std=c++14 -g -Iinclude src/bison_types.cpp src/bison.cpp src/main.cpp -o $(basename $@)

test: bison unit_test sample_test

unit_test: bison $(unit_tests)

sample_test: bison $(sample_tests)

$(unit_tests):
	@echo
	@echo $(L)
	@echo "=     $@"
	@echo $(L)
	@./build/bison $(addsuffix .bson, ./test/$@) #$(addsuffix .json, ./test/$@)
	@echo $(L)


$(sample_tests):
	@echo
	@echo $(L)
	@echo "=     $@"
	@echo $(L)
	@./build/bison $(addsuffix .bson, ./test/$@) #$(addsuffix .json, ./test/$@)
	@echo $(L)


clean:
	@rm -rf ./build
