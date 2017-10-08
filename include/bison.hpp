#ifndef BISON_HPP
#define BISON_HPP

#include "bison_objects.hpp"

#include <vector>

/*
main class to use the bson parser
this class implements the specification v1.1 of the bson spec

how to use loads a bsonfile in memory using a std::vector<char>
the file MUST be a complete and valid bson file
invalid bson file will result on hard exit

see unit_tests for the accessor of each type

*/

class Bson{
public:
    Bson() = delete;
    ~Bson();
    Bson(std::vector<char>& vect);
    BsonDoc* getDoc();               //get the current document as BsonDoc or null if there is no more
    void next();                     //jump to the next document in the file
    std::string dump();              //dump a human readable version of the current document as std::string
private:
    BsonDoc*                    _doc = nullptr;
    std::vector<char>::iterator _pos;
    std::vector<char>&          _vect;
};

#endif // ndef BISON_HPP
