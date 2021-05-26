#include <vector>
using namespace std;

struct Node{
    int color;
    int height;
    pair<int, int> start;
    pair<int, int> end;
    Node* children[4];
    Node() {}
    Node(pair<int, int> start, pair<int, int> end, int color){
        this->start = start;
        this->end = end;
        this->color = color;
    }

    bool checkColor(){
        int childColor = children[0]->color;
        for(int i = 1; i < 4; ++i)
            if(children[i])
                if(children[i]->color != childColor) return false;
        return true;
    }

    void compress(){
        if(checkColor()){
            for(int i = 0; i < 4; ++i)
                if(children[i])
                    delete children[i];
        }
    }
};