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

    vector<vector<int>> getMatrix(fstream& file, int height, int width){
        vector<vector<int>> matrix(height, vector<int>(width));
        for(int i = 0; i < height; ++i)
            for(int j = 0; j < width; ++j)
                file >> matrix[i][j];
        return matrix;
    }

    void printMatrix(vector<vector<int>>& matrix){
        for(auto& row : matrix){
            for(auto& value : row){
                cout << setw(3) << value;
            }
            cout << endl;
        }
    }

    void insert();

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
            auto matrix = getMatrix(file, height, width);
            printMatrix(matrix);
            file.close();
            buildTree(matrix);
        }else cout << "No existe" << endl;
    }

    void buildTree(vector<vector<int>>& matrix){
        
    }
};