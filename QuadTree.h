#ifndef EDA_QUADTREE_H
#define EDA_QUADTREE_H

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Node.h"
#include <cstdlib>
using namespace std;

class CompressFile;

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

    Node* buildTreeUtil(pair<int, int> start, pair<int, int> end){
        Node* node = new Node(start, end);
        if(start.first == end.first){
            if(start.second == end.second){
                node->color = matrix[start.first][start.second];
                return node;
            }
            else{
                int step = (end.second - start.second)/2;
                node->children[0] = buildTreeUtil(start, {start.first, start.second+step});
                node->children[2] = buildTreeUtil({start.first, start.second+step+1}, end);
            }
        }
        else if(start.second == end.second){
            int step = (end.first - start.first)/2;
            node->children[0] = buildTreeUtil(start, {start.first+step, start.second});
            node->children[1] = buildTreeUtil({start.first+step+1, start.second}, end);
        }
        else{
            int stepHeight = (end.first - start.first)/2;
            int stepWidth = (end.second - start.second)/2;
            node->children[0] = buildTreeUtil(start, {start.first+stepHeight, start.second+stepWidth});
            node->children[1] = buildTreeUtil({start.first, start.second+stepWidth+1}, {start.first+stepHeight, end.second});
            node->children[2] = buildTreeUtil({start.first+stepHeight+1, start.second}, {end.first, start.second+stepWidth});
            node->children[3] = buildTreeUtil({start.first+stepHeight+1, start.second+stepWidth+1}, end);
        }
        node->compress();
        return node;
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
            file.close();
            buildTree();
        }else cout << "Error al abrir el archivo" << endl;
    }

    void buildTree(){
        this->root = buildTreeUtil({0,0}, {height-1, width-1});
    }

    void generatePDF(string folder) {
        string graphFile = folder + "graph.vz";
	    fstream file(graphFile, fstream::out | fstream::trunc);
		if (file.is_open()) {
			file << "digraph G\n" << "{\n";
			root->printAllNodes(file);
			root->printNodesConexiones(file);
			file << "}\n";
			file.close();
            string command = "dot -Tpdf " + folder + "graph.vz" + " -o " + folder + "graph.pdf";
            cout << command << endl;
			system(command.c_str());
		}
	}

    friend class CompressFile;
};

#endif