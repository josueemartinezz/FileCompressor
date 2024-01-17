#include <unordered_map>
#include <vector>
#include <queue>
#include "Helper.hpp"
#include "HCTree.hpp"
#include <bits/stdc++.h> 
using namespace std;
int main(int argc, char* argv[]) {
    FancyInputStream file = FancyInputStream(*(argv+1));
    FancyOutputStream file2 = {argv[2]};
    if(file.filesize() == 0){
        // file2.write_byte(file.read_byte());
        return 0;
    }
    if(file.filesize() == -1){
        const char* message = "file does not exist.";
        error(message);
    }
    file.reset();
    HCTree huffman = HCTree();
    int start = 1;
    // huffman.deserialize(start, file.filesize()*file.filesize(), file);
    huffman.deserialize(start, file.filesize(), file);
    int index = file.read_int();
    int count = 0;
    while(count < index){
        count++;
        unsigned char code = huffman.decode(file);
        if(code){
            file2.write_byte(code);
        }
    }

}
