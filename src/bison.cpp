#include "bison.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <ctime>

#include <iostream>

Bson::Bson(std::vector<char>& vect) {
    char* buff = vect.data();
    _doc = new BsonDoc(buff, vect.size());
};

Bson::~Bson(){
    std::vector<BsonObj*> toDelete;
    BsonObj* doc = getDoc();
    toDelete.push_back(doc);
    while( (doc=doc->next()) ) {
        toDelete.push_back(doc);
    }

    for (const BsonObj* d: toDelete) {
        delete d;
    }
}

std::string Bson::dump() {
    std::string str;
    BsonObj* doc = getDoc();
    if (doc != nullptr) {
        str += doc->dump();
        while( (doc=doc->next()) ) 
        {
            str.append(doc->dump());
        }
    }
    return str;
}

BsonDoc* Bson::getDoc()
{
    if (_doc == nullptr) {
        std::cerr << "fatal Bson object not initialized" << std::endl;
        exit(1);
    }
    return _doc;
}
