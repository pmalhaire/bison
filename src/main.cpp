#include <iostream>
#include <vector>
#include <fstream>
#include <sys/stat.h>

#include "bison.hpp"

using namespace std;

void usage() {
    cerr << "bison takes one file path as argument" << endl;
    cerr << "$bison ./hello.bson";
}

long getFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char *argv[]) {
    string file_path;
    vector<char> fileVector;

    //test arg number 
    if (argc != 2) {
        cerr << "Wrong arg numbers";
        usage();
    }

    file_path = string(argv[1]);
    long fileSize = getFileSize( file_path );

    if ( fileSize <= 0 ) {
        cerr << file_path << " is empty" << endl ;
        return 1;
    }

    ifstream fileStream(file_path, std::ios::binary);




    //check that the file can be opened
    if ( !fileStream.is_open() ) {
        cerr << file_path << " could not be opened" << endl ;
        return 1;
    }

    //check readability
    if ( !fileStream.good() ) {
        cerr << file_path << " is not readable" << endl ;
        return 1;
    }

    cout << file_path << " loaded" << endl;
    
    fileVector.reserve(fileSize);
    fileVector.assign(std::istreambuf_iterator<char>(fileStream),
                        std::istreambuf_iterator<char>());

    fileStream.close();

    Bson test(fileVector);

    //parse one by one
    //TODO
    while(test.getDoc() != nullptr) {
        //dump content
        std::cout << test.dump();
        test.next();
    }
}
