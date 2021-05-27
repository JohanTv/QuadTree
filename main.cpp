#include "QuadTree.h"

void run(string filename, bool generatePDF = false){
    string carpeta = "tests/"+filename+"/";
    QuadTree test(carpeta+filename+".pgm");
    if(generatePDF) test.generatePDF(carpeta);
    test.writeFileCompressed(carpeta+"output_"+filename+".dat");
    test.readQuadTree(carpeta+"output_"+filename);
}


int main(){
    run("casinoTest");
    run("dragon", true);
    run("lena.ascii");
    run("tablero");
}
