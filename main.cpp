#include "QuadTree.h"

int main(){
    QuadTree test("dragon.pgm");
    test.writeFileCompressed("output.dat");
    test.readQuadTree("output.dat");
}