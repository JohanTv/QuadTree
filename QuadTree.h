#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Node.h"
#include <stdlib.h>
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

    void fillMatrix(fstream& file, vector<vector<int>>& matrix){
        while(!file.eof()){
            usefulInfomation info;
            file.read((char*)& info, sizeof(info));
            pair<int, int> start = info.start;
            pair<int, int> end = info.end;
            for(int i = start.first; i <= end.first; ++i)
                for(int j = start.second; j <= end.second; ++j)
                    matrix[i][j] = info.color;
        }
    }

    void writeMatrix(fstream& file, vector<vector<int>>& matrix){
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                file << matrix[i][j];
                if(j != width - 1) file << " "; 
            }
            if(i != height - 1) file << '\n';
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
            printMatrix();
            file.close();
            buildTree();
        }else cout << "Error al abrir el archivo" << endl;
    }

    void buildTree(){
        this->root = buildTreeUtil({0,0}, {height-1, width-1});
    }

    void generatePDF() {
	    fstream file("graph.vz", fstream::out | fstream::trunc);
		if (file.is_open()) {
			file << "digraph G\n" << "{\n";
			root->printAllNodes(file);
			root->printNodesConexiones(file);
			file << "}\n";
			file.close();
			system("dot -Tpdf graph.vz -o graph.pdf");
		}
	}

    void writeFileCompressedUtil(fstream& file, Node* node){
        if(node->color != -1){
            usefulInfomation info;
            info.color = node->color;
            info.start = node->start;
            info.end = node->end;
            file.write((char*)& info, sizeof(info));
            return;
        }
        for(int i = 0; i < CHILDREN_NUMBER; ++i){
            if(node->children[i]) writeFileCompressedUtil(file, node->children[i]);
        }
    } 

    void writeFileCompressed(string filename){
        fstream file(filename, ios::out | ios::binary);
        file.write((char*)& width, sizeof(width));
        file.write((char*)& height, sizeof(height));
        file.write((char*)& maxGrayScale, sizeof(maxGrayScale));
        writeFileCompressedUtil(file, this->root);
        file.close();
    }
  
    void readQuadTree(string filename){
        fstream file(filename, ios::in | ios::binary);
        if(file.is_open()){
            fstream pgmFile("nuevaImagen.pgm", ios::out);
            int width, height, maxGrayScale_;
            file.read((char*)& width, sizeof(width));
            file.read((char*)& height, sizeof(height));
            file.read((char*)& maxGrayScale_, sizeof(maxGrayScale_));
            pgmFile << "P2\n# feep.pgm\n"; 
            pgmFile << width << " " << height << '\n';
            pgmFile << maxGrayScale_ << '\n';
            vector<vector<int>> newMatrix(height, vector<int>(width, 0));
            fillMatrix(file, newMatrix);
            writeMatrix(pgmFile, newMatrix);
            file.close();
            pgmFile.close();
        }else cout << "No se puede abrir el archivo" << endl;
    }
};