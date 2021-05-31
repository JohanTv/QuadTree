#include "QuadTree.h"
#include "CompressFile.h"

void run(string filename, bool generatePDF = false){
    string carpeta = "CurrentResults/"+filename+"/";
    QuadTree test(carpeta+filename+".pgm");
    if(generatePDF) test.generatePDF(carpeta);
    CompressFile file(test);
    file.writeFileCompressed(carpeta+"output_"+filename+".dat");
    file.rebuildImage(carpeta+"output_"+filename);
}


int main(){
    run("casinoTest");
    run("dragon");
    run("lena.ascii");
    run("tablero");
}
