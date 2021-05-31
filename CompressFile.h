#ifndef EDA_COMPRESSFILE_H
#define EDA_COMPRESSFILE_H

#include <vector>
#include <unordered_map>
#include "QuadTree.h"
using namespace std;

typedef pair<short, short> coord;

struct Util{
    vector<coord> points;
    vector<pair<coord, coord>> pointsSection;
    Util(coord start, coord end){
        add(start, end);
    }
    void add(coord start, coord end){
        if(start == end)
            points.push_back(start);
        else
            pointsSection.emplace_back(start, end);
    }

    void save(fstream& file){
        int size = points.size();
        file.write((char *)& size, sizeof(size));
        for(int i = 0; i < size; ++i){
            file.write((char *)& points[i], sizeof(points[i]));
        }
        size = pointsSection.size();
        file.write((char *)& size, sizeof(size));
        for(int i = 0; i < size; ++i){
            file.write((char *)& pointsSection[i], sizeof(pointsSection[i]));
        }
    }
};

class CompressFile{
private:
    unordered_map<short, Util*> myMap;
    QuadTree quadTree;
    void saveLeavesInformationUtil(Node* node){
        if(node->color != -1){
            this->add(node->color, node->start, node->end);
            return;
        }
        for(int i = 0; i < CHILDREN_NUMBER; ++i){
            if(node->children[i]) saveLeavesInformationUtil(node->children[i]);
        }
    }

public:
    explicit CompressFile(QuadTree quadTree_) : quadTree(quadTree_) {}
    void add(short color, coord start, coord end){
        if(myMap.find(color) == myMap.end()){
            myMap[color] = new Util(start, end);
        }else{
            myMap[color]->add(start, end);
        }
    }
    void saveLeavesInformation(){
        saveLeavesInformationUtil(quadTree.root);
    }

    void saveMap(fstream& file){
        for(auto it : myMap){
            file.write((char *)& it.first, sizeof(it.first));
            it.second->save(file);
        }
    }

    void writeFileCompressed(string filename){
        saveLeavesInformation();
        fstream file(filename, ios::out | ios::binary);
        file.write((char*)& quadTree.width, sizeof(quadTree.width));
        file.write((char*)& quadTree.height, sizeof(quadTree.height));
        file.write((char*)& quadTree.maxGrayScale, sizeof(quadTree.maxGrayScale));
        short size = myMap.size();
        file.write((char*)& size, sizeof(size));
        saveMap(file);
        file.close();
    }

    void fillInPoints(vector<vector<int>>& matrix, vector<coord> points, short color){
        for(int i = 0; i < points.size(); ++i){
            matrix[points[i].first][points[i].second] = color;
        }
    }

    void fillInPointsSection(vector<vector<int>>& matrix, vector<pair<coord, coord>> pointsSection, short color){
        for(int m = 0; m < pointsSection.size(); ++m){
            pair<int, int> start = pointsSection[m].first;
            pair<int, int> end = pointsSection[m].second;
            for(int i = start.first; i <= end.first; ++i)
                for(int j = start.second; j <= end.second; ++j)
                    matrix[i][j] = color;
        }
    }

    void fillMatrix(fstream& file, vector<vector<int>>& matrix){
        short total;
        file.read((char *)& total, sizeof(total));
        short color;
        int i = 0;
        while(i != total){
            file.read((char *)& color, sizeof(color));
            int size1, size2;
            file.read((char *)& size1, sizeof(size1));
            if(size1 != 0){
                coord points[size1];
                file.read((char *)& points, size1*sizeof(coord));
                fillInPoints(matrix, vector<coord>(points, points+size1), color);
            }
            file.read((char *)& size2, sizeof(size2));
            if(size2 != 0){
                pair<coord,coord> pointsSection[size2];
                file.read((char *)& pointsSection, size2*sizeof(pair<coord, coord>));
                fillInPointsSection(matrix, vector<pair<coord, coord>>(pointsSection, pointsSection+size2), color);
            }
            ++i;
        }
    }

    void writeMatrix(fstream& file, vector<vector<int>>& matrix){
        for(int i = 0; i < matrix.size(); ++i){
            for(int j = 0; j < matrix[i].size(); ++j){
                file << matrix[i][j];
                if(j != matrix[i].size() - 1) file << " ";
            }
            if(i != matrix.size() - 1) file << '\n';
        }
    }

    void rebuildImage(string filename){
        fstream file(filename+".dat", ios::in | ios::binary);
        if(file.is_open()){
            fstream pgmFile(filename+".pgm", ios::out);
            int width, height, maxGrayScale_;
            file.read((char*)& width, sizeof(width));
            file.read((char*)& height, sizeof(height));
            file.read((char*)& maxGrayScale_, sizeof(maxGrayScale_));
            pgmFile << "P2\n# feep.pgm\n";
            pgmFile << width << " " << height << '\n';
            pgmFile << maxGrayScale_ << '\n';
            vector<vector<int>> matrix(height, vector<int>(width, 0));
            fillMatrix(file, matrix);
            writeMatrix(pgmFile, matrix);
            file.close();
            pgmFile.close();
        }else cout << "No se puede abrir el archivo" << endl;
    }

};

#endif