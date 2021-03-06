#ifndef UTILS_CPP
#define UTILS_CPP

#include <chrono>
#include "kernel/kernelfactory.h"
using namespace std::chrono;

//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

template<class T>
bool vector_contains(vector<T> &v, T el){
  for(typename std::vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
    if(*it == el){
      return true;
    }
  }
  return false;
}



vector<Graph*> loadData(const char *file, LabelType labelType){
  vector<Graph*> graphs;

  string line;
  ifstream input(file);
  int index = 0;
  if(input.is_open()){
    Graph *g;
    int n;
    int i;
    enum State {start, labels, edges};
    State s = start;
    while(getline(input, line)){
      switch(s){
      case start:
        n = atoi(line.c_str());
        g = new Graph(n);
         g->index = index;
         index++;
        graphs.push_back(g);
        s = labels;
        i = 0;
        break;
      case labels:
        switch(labelType){
        case LabelType::Discrete:
          g->V[i]->dLabel = atoi(line.c_str());
          break;
        case LabelType::Vector:
          vector<string> vLabel = split(line, ' ');
          for(int j=0; j<vLabel.size(); j++){
            g->V[i]->vLabel.push_back(stod(vLabel[j]));
          }
          break;
        }
        i++;
        if(i == n){
          s = edges;
        }
        break;
      case edges:
        if(line == "stop"){
          s = start;
        }else{
          vector<string> edge = split(line, ',');
          int a = atoi(edge[0].c_str());
          int b = atoi(edge[1].c_str());
          g->V[a]->adj.push_back(g->V[b]);
        }
      }
    }
  }

  return graphs;
}

void writeKToFile(double **K, int size){
  ofstream outputFile;
  outputFile.open("output/K.dat");
  for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){
      outputFile << K[i][j] << " ";
    }
    outputFile << "\n";
  }
  outputFile.close();
}

double msPassed(steady_clock::time_point tStart){
  return duration_cast<duration<double>>(steady_clock::now() - tStart).count() * 1000;
}


bool nodeCompare(Node *a, Node *b){
  return a->tmp_d > b->tmp_d;
}

#endif
