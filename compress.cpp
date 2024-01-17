#include <unordered_map>
#include <vector>
#include <queue>
#include "Helper.hpp"
#include "HCTree.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    FancyInputStream file = FancyInputStream(*(argv+1));
    FancyOutputStream fileOut =  {argv[2]};
    if(file.filesize() == 0){
        // fileOut.write_byte(file.read_byte());
        return 0;
    }
    cout << "access" << endl;
    if(file.filesize() == -1 || argc != 3){
        const char* message = "Incorrect parameters \nUSAGE: ./refcompress <original_file> <compressed_file>";
        error(message);
    }
    vector<int> freqs = vector<int>(256);
    for(int i = 0; i < 256; i++) {
        freqs[i] = 0;
    }
    int word = file.read_byte();
    int index = 0;
    while(word != -1){
        index++;
        freqs[word]++;
        word = file.read_byte();
    }
    HCTree huffman = HCTree();
    huffman.build(freqs, fileOut);
    fileOut.write_int(index);
    file.reset();
    while(file.good()){
        huffman.encode(file.read_byte(), fileOut);
    }

}
