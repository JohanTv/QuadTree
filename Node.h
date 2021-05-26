#include <vector>
#define CHILDREN_NUMBER 4
using namespace std;

struct usefulInfomation{
    short color;
    pair<short, short> start;
    pair<short, short> end;
};

struct Node{
    int color;
    pair<int, int> start;
    pair<int, int> end;
    Node* children[CHILDREN_NUMBER];
    Node() {}
    Node(pair<int, int> start, pair<int, int> end, int color){
        this->start = start;
        this->end = end;
        this->color = color;
        initializeChildren();
    }
    Node(pair<int, int> start, pair<int, int> end){
        this->start = start;
        this->end = end;
        this->color = -1;
        initializeChildren();
    }

    void initializeChildren(){
        for(int i = 0; i < CHILDREN_NUMBER; ++i)
            children[i] = nullptr;
    }
    
    bool checkColor(){
        for(int i = 0; i < CHILDREN_NUMBER; ++i)
            if(children[i] && children[i]->color == -1) return false;
            
        int childColor = children[0]->color;
        for(int i = 1; i < CHILDREN_NUMBER; ++i)
            if(children[i] && children[i]->color != childColor) return false;
        return true;
    }

    void compress(){
        if(checkColor()){
            this->color = this->children[0]->color;
            for(int i = 0; i < CHILDREN_NUMBER; ++i)
                if(children[i]) delete children[i];
            initializeChildren();
        }
    }

    void printLimits(fstream& file){
        file << "(" << this->start.first << ", " << this->start.second << ") - ";
        file << "(" << this->end.first << ", " << this->end.second << ")"; 
    }

    void printNodesConexiones(fstream &file){
        for(int i = 0; i < CHILDREN_NUMBER; ++i){
            if(this->children[i]){
                file << "\"" << this << "\"->";
                file << "\"" << this->children[i] << "\";\n"; 
                this->children[i]->printNodesConexiones(file);
            }
        }
	}

    void printAllNodes(fstream &file){
		file << "\"" << this << "\" [\n";
        file << "\tlabel = \"" << "\\n Color: " << this->color; 
        file <<"\\n Limites: "; this->printLimits(file);
        file << " \"\n]\n";
		for(int i = 0; i < CHILDREN_NUMBER; ++i){
            if(this->children[i])
                this->children[i]->printAllNodes(file);
        }
	}
};