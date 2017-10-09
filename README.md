# Bison is a basic Bson parser based on a vector

Still at early stage but working 

most types are implement

##build 

```bash
make


````

##test 

make test

##debug

to

##use

```bash
❯ ./build/$(uname)/bison test/hello.bson                                                                                                                                                                      [00:39:29]
test/hello.bson loading size:22
>> test/hello.bson loaded
{
"hello" : "world"
}
````