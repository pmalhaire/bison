#include <iostream>
#include <vector>
#include <fstream>


#include "bison.hpp"

using namespace std;

void usage() {
    cerr << "bison takes one file path as argument" << endl;
    cerr << "$bison ./hello.bson";
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

    //test file existance
    ifstream is(file_path, std::ios::binary);

    //check that the file can be opened
    if ( !is.is_open() ) {
        cerr << file_path << " could not be opened" << endl ;
        return 1;
    }

    //check empty
    if ( !is.good() ) {
        cerr << file_path << " is empty or not readable" << endl ;
        return 1;
    }

    cout << file_path << " loaded" << endl;
    
    //TODO make this cleaner
    while (is.good() && !is.eof()) {
        char c = is.get();
        if ( !is.eof() ) fileVector.emplace_back(c);
    }
    is.close();

    /*for (auto& c : fileVector) {
        cout << c;  
    }
    cout << endl;
    */
    Bson test(fileVector);

    //parse one by one
    //TODO
    while(test.getDoc() != nullptr) {
        //dump content
        std::cout << test.dump();
        test.next();
    }
}
