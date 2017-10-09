# Bison is a basic Bson parser based on a vector

implementing bson spec v1.1 see http://bsonspec.org/spec.html 

## Note

The code is still at early stage but the 22 types are implemented.

To see how types are implemented see [bison_objects.hpp](include/bison_objects.hpp)

Some accessors may need update. 
Some types do not have a proper dump function BsonID for example. 

## parsing example

For parsing example see [accessors_tests.cpp](src/accessors_tests.cpp).

## platforms

Tested under linux (Ubuntu 17.04), osx(10.13) and windows(windows 10 using msys/mingw64)

## build 

```bash
make
 ❯ bison build OK
```

## test 

```bash
make test
```

## debug

Build with debug symbols using `DEBUG=1`.

ex 
```bash
make DEBUG=1 test
```
or to keep debug active
```bash
export DEBUG=1
make test
```

## use

```bash
./build/$(uname)/bison test/hello.bson
❯ {
❯ "hello" : "world"
❯ }
```

## test more

You can run the parser on a big file ~1GB using.
It's justs parse the file the parse result is not checked.

```bash
make big_test
```
