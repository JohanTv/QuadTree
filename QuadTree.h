#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Node.h"
using namespace std;

class QuadTree {
private:
    Node* root;
    int height;
    int width;
    int maxGrayScale;
    vector<vector<int>> matrix;

    void generateMatrix(fstream& file, int height, int width){
        matrix =  vector<vector<int>>(height, vector<int>(width));
        for(int i = 0; i < height; ++i)
            for(int j = 0; j < width; ++j)
                file >> matrix[i][j];
    }

    void printMatrix(){
        for(auto& row : matrix){
            for(auto& value : row){
                cout << setw(3) << value;
            }
            cout << endl;
        }
    }
public:
    QuadTree(){}
    QuadTree(string filename){
        readPGMFile(filename);
    }
    
    void readPGMFile(string filename){
        fstream file(filename, ios::in);
        if(file.is_open()){
            string comment;
            getline(file, comment);
            getline(file, comment);
            file >> width >> height >> maxGrayScale;
            generateMatrix(file, height, width);
            printMatrix();
            file.close();
            buildTree();
        }else cout << "Error al abrir el archivo" << endl;
    }

    void buildTree(){
        buildTreeUtil({0,0}, {height-1, width-1});
    }

    void buildTreeUtil(pair<int, int> start, pair<int, int> end){
        if(start.first == end.first){
            if(start.second == end.second){
                insert(this->root, matrix[start.first][start.second]);
            }
            else{
                int step = (end.second - start.second)/2;
                buildTreeUtil(start, {start.first, start.second+step});
                buildTreeUtil({start.first, start.second+step+1}, end);
            }
            return;
        }
        if(start.second == end.second){
            int step = (end.first - start.first)/2;
            buildTreeUtil(start, {start.first+step, start.second});
            buildTreeUtil({start.first+step+1, start.second}, end);
            return;
        }
        int stepHeight = (end.first - start.first)/2;
        int stepWidth = (end.second - start.second)/2;
        buildTreeUtil(start, {start.first+stepHeight, start.second+stepWidth});
        buildTreeUtil({start.first, start.second+stepWidth+1}, {start.first+stepHeight, end.second});
        buildTreeUtil({start.first+stepHeight+1, start.second}, {end.first, start.second+stepWidth});
        buildTreeUtil({start.first+stepHeight+1, start.second+stepWidth+1}, end);
    }

    void insert(Node* root, int grayScale){
        // falta implementar
    }
};