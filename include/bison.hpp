#include "bison_types.hpp"

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


