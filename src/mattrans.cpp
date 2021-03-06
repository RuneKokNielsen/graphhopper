
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "mat.h"
#include "matrix.h"
using namespace std;


int transform(const char *file){
  cout << "Input file: " << file << "\n";

  MATFile *matFile = matOpen(file, "r");
  if(matFile == NULL){
    std::cout << "Error opening file: " << file << "\n";
    return 1;
  }

  mxArray *data;
  const char *name;
  data = matGetNextVariable(matFile, &name);
  if(data == NULL){
    cout << "File empty\n";
    return 1;
  }

  cout << "Data header name: " << name << "\n";

  mwSize nGraphs = (mwSize) mxGetNumberOfElements(data);

  cout << "Number of graphs: " << nGraphs << "\n";

  ofstream outputFile;
  outputFile.open("output/output.dat");
  if(!outputFile.is_open()){
    cout << "Could not open output file!\n";
    return 1;
  }

  const mxArray *am;
  const mxArray *nlc;
  const mxArray *nl;
  const double *m;
  mxArray *row;
  mwIndex *ir, *jc;
  mwIndex starting_row_index, stopping_row_index, current_row_index;
  for(int i=0; i<nGraphs; i++){

    am = mxGetField(data, i, "am");
    nlc = mxGetField(data, i, "nl");
    nl = mxGetField(nlc, 0, "values");
    if(nl == NULL) nl = mxGetField(nlc, 0, "vecvalues");
    if(nl == NULL){
      cout << "Unknown label value name in graph " << i
           << "\nOptions are: values, vecvalues\n";
    }
    mwSize nNodes = (mwSize) mxGetM(am);

    outputFile << nNodes << "\n";

    m = mxGetPr(nl);
    for(int row=0; row<nNodes; row++){
      outputFile << m[row] << "\n";
    }
    m = mxGetPr(am);
    if(mxIsSparse(am)){
      //Sparse am matrix
      int total = 0;

      ir = mxGetIr(am);
      jc = mxGetJc(am);

      for(int col=0; col<nNodes; col++){
        starting_row_index = jc[col];
        stopping_row_index = jc[col+1];
        for (current_row_index = starting_row_index;
             current_row_index < stopping_row_index;
             current_row_index++)  {
          outputFile << ir[current_row_index] << "," << col << "," << m[total++] << "\n";
        }
      }
    }else{
      //Regular am matrix
      for(int x=0; x<nNodes; x++){
        for(int y=0; y<nNodes; y++){
          double v = m[x + y * nNodes];
          if(v > 0){
            outputFile << x << "," << y << "\n";
          }
        }
      }
    }
    outputFile << "stop\n";
  }


  outputFile.close();
  cout << "Data written!\n";
  return 0;
}

int main(int argc, char **argv){

  cout << "MAT-file translator for <GraphHopper>\n";
  int result = 0;
  if(argc > 1){
    result = transform(argv[1]);
  }else{
    cout << "Usage: mattrans <matfile>\n";
    result = 1;
  }

  return result;
}
