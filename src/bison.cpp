#include "bison.hpp"

#include <ctime>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace bison {
std::unique_ptr<BsonDoc> Bson::next() {
    auto remaining = std::distance(_pos, _vect.end());
    if (remaining > 0) {
        std::unique_ptr<BsonDoc> doc =
            std::make_unique<BsonDoc>(_pos, remaining);
        _pos = doc->end();
        return doc;
    }
    return nullptr;
}
} // namespace bison
