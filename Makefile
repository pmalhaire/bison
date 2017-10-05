unit_tests = hello timestamp

sample_tests = mongodump

.PHONY: clean bison test unit_test sample_test $(unit_tests) $(sample_tests)

bison: build/bison

build/bison: src/main.cpp src/bison.cpp include/bison.hpp
	mkdir -p build
	g++ -Wall -std=c++14 -g -Iinclude src/bison.cpp src/main.cpp -o $(basename $@)

test: bison unit_test sample_test

unit_test: bison $(unit_tests)

sample_test: bison $(sample_tests)

$(unit_tests):
	@echo $@
	@echo =====================================
	@./build/bison $(addsuffix .bson, ./test/$@) #$(addsuffix .json, ./test/$@)


$(sample_tests):
	@echo $@
	@echo =====================================
	@./build/bison $(addsuffix .bson, ./test/$@) #$(addsuffix .json, ./test/$@)


clean:
	@rm -rf ./build
