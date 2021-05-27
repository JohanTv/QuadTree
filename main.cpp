#include "QuadTree.h"

void run(string filename){
    string carpeta = "tests/"+filename+"/";
    QuadTree test(carpeta+filename+".pgm");
    test.writeFileCompressed(carpeta+"output_"+filename+".dat");
    test.readQuadTree(carpeta+"output_"+filename);
}


int main(){
    //QuadTree test("dragon.pgm");
    //QuadTree test("lena.ascii.pgm");
    //QuadTree test("casinoTest.pgm");
    run("lena.ascii");
}
