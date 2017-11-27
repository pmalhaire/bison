#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <vector>

#include "bison.hpp"

using namespace bison;

void usage() {
    std::cerr << "bison takes one file path as argument" << std::endl;
    std::cerr << "$bison ./hello.bson";
}

long getFileSize(std::string filename) {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char *argv[]) {
    std::string file_path;
    std::vector<char> fileVector;

    // test arg number
    if (argc != 2) {
        std::cerr << "Wrong arg numbers";
        usage();
    }

    file_path = std::string(argv[1]);
    long fileSize = getFileSize(file_path);

    if (fileSize <= 0) {
        std::cerr << file_path << " is empty" << std::endl;
        return 1;
    }

    std::ifstream fileStream(file_path, std::ios::binary);

    // check that the file can be opened
    if (!fileStream.is_open()) {
        std::cerr << file_path << " could not be opened" << std::endl;
        return 1;
    }

    // check readability
    if (!fileStream.good()) {
        std::cerr << file_path << " is not readable" << std::endl;
        return 1;
    }

    fileVector.reserve(fileSize);
    fileVector.assign(std::istreambuf_iterator<char>(fileStream),
                      std::istreambuf_iterator<char>());
    fileStream.close();
    // load the vector into the parser
    Bson test(fileVector);

    std::unique_ptr<BsonDoc> doc;
    // parse all documents in the file
    while ((doc = test.next())) {
        // dump the content of each document to console
        std::cout << doc->dump() << std::flush;
    }
}
