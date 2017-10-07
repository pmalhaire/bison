#ifndef BISON_HPP
#define BISON_HPP

#include "bison_objects.hpp"

#include <vector>

/*
main class to use the bson parser
*/

class Bson{
public:
    Bson() = delete;
    ~Bson();
    Bson(std::vector<char>& vect);
    std::string dump();
    BsonDoc* getDoc();
private:
    BsonDoc* _doc = nullptr;
};

#endif // ndef BISON_HPP
