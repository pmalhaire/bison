# Bison is a basic Bson parser based on a vector

implementing bson spec v1.1 see http://bsonspec.org/spec.html 

the code is still at early stage but the 22 types are implement
some accessors may have to be updated
some dump may need cleanup

## build 

```bash
make                                                                                                                                      [13:33:25]
 ❯ bison build OK
```

## test 

```bash
make test
```

## debug

build with debug symbols using DEBUG=1

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

you can run the parser on a big file ~1GB using

```bash
make big_test
```
