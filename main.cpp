#include <iostream>
#include <vector>
#include "Algorithms.h"
#include <filesystem>

void generateRandK(int k, int len, string outputFilename){
    ofstream output(outputFilename);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFilename << std::endl;
        return;
    }
    for (int i=0;i<len;++i){
        output << (char)(rand() % k);
    }
    output.close();
}

int main(){
    srand(time(nullptr));
//    generateRandK(2,1e5,"tests/rand2/rand2.txt");
//    generateRandK(32,1e5,"tests/rand32/rand32.txt");
//    generateRandK(64,1e5,"tests/rand64/rand64.txt");
//    return 0;
    Algorithms algorithms;
    algorithms.TestAllAlgorithms();
}
