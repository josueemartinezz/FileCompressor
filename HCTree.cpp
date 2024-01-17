#include <queue>
#include <stack>
#include <vector>
#include <fstream>
#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;
 
void helperClear(HCNode* &current);

HCTree::~HCTree(){
    helperClear(root);
    root = nullptr;
}
void helperClear(HCNode* &current){
    if(current == nullptr){
        return;
    }
    helperClear(current->c0);
    helperClear(current->c1);
    delete(current);
}
void HCTree::serialize(HCNode* current, FancyOutputStream & out){
    if(current == nullptr){
        return;
    } 
    if(current->c0 == nullptr && current->c1 == nullptr){
        out.write_bit(1);
        out.flush_bitwise();
        out.write_byte(current->symbol);
    }else{
        out.write_bit(0);
        serialize(current->c0, out);
        serialize(current->c1, out);
    }
}

HCNode* HCTree::deserialize(int &index, int len, FancyInputStream & in){
    if(index >= len) {
        return nullptr;
    }
    int bit = in.read_bit();
    if(bit == 0){
        if(root){
            HCNode* curr = new HCNode(0,0);
            curr->c0 = deserialize(++index, len, in);
            curr->c1 = deserialize(++index, len, in);
            return curr;
        }else{
            root = new HCNode(0,0);
            root->c0 = deserialize(++index, len, in);
            root->c1 = deserialize(++index, len, in);
            return root;
        }
    }else{
        while(index % 8 != 0){
            index++;
            in.read_bit();
        }
        unsigned char c = in.read_byte();
        if(root){
            HCNode* curr = new HCNode(0, c);
            return curr;
        }else{
            root = new HCNode(0, c);
            return root;
        }
    }
}

void HCTree::build(const vector<int>& freqs, FancyOutputStream & out){
    vector<int>::const_iterator itt = freqs.begin();
    // Frequency Node Creation
    unsigned char i = 0;
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    while(itt != freqs.end()) {
            if(*itt){
                HCNode* curr = new HCNode(*itt, i);
                leaves[i] = curr;
                pq.push(curr);
                i++;
                itt++;
            }else{
                i++;
                itt++;
            }
    }
    while(pq.size() > 1){
        HCNode *lowest = pq.top();
        pq.pop();
        HCNode *second = pq.top();
        pq.pop();
        HCNode* newNode = new HCNode((lowest->count + second->count), 0);
        newNode->c0 = lowest;
        newNode->c1 = second; 
        lowest->p = newNode; 
        second->p = newNode; 
        pq.push(newNode);
    }   
    root = pq.top();
    serialize(root, out);
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const  {
    HCNode *found;
    for(auto const &element : leaves){
            if(element){
                if(symbol == element->symbol){
                    found = element;
                    break;
                }
            }
    }
    stack<int> list;
    HCNode* temp;
    while(found != root){
        temp = found;
        found = found->p;
        if(found->c0 == temp){
            list.push(0);
        }else{
            list.push(1);
        }
    }
    int i;
    while(!list.empty()){
        i = list.top();
        out.write_bit(i);
        list.pop();
    }
}

unsigned char HCTree::decode(FancyInputStream &in) const {
    unsigned char result = 0;
    HCNode* curr = root;
    int byte = in.read_bit();
    if(byte == -1){
        return 0;
    }
    while(curr){
        if(byte){
            if(curr->c1) {
                curr = curr->c1;
                if(curr->c1 || curr->c0){
                    byte = in.read_bit();
                    if(byte == -1){
                        return 0;
                    }
                }
            }else{
                return curr->symbol;
            }
        }else{
            if(curr->c0) {
                curr = curr->c0;
                if(curr->c0 || curr->c1){
                    byte = in.read_bit();
                    if(byte == -1){
                        return 0;
                    }
                }
            }else{
                return curr->symbol;
            }
        }
    }
    return result;
}
