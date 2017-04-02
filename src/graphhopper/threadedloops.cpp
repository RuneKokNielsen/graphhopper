#include "threadedloops.h"
#include <thread>

#ifdef MEX
#include "mex.h"
#endif

ThreadedLoops::ThreadedLoops(int nThreads){
  _nThreads = nThreads;
  _calculated = 0;
}

void ThreadedLoops::computeInterval(double **K, vector<Graph*> *pGraphs,
                                    NodeKernel *kernel,
                                    int from, int to){

  vector<Graph*>& graphs = *pGraphs;
  for(int gii=from; gii<=to; gii++){
    for(int gji=gii; gji<_nGraphs; gji++){
      Graph *gi = graphs[gii];
      Graph *gj = graphs[gji];
      double sum = 0;
      if(gji < gii){ //Using symmetric property
        sum = K[gji][gii];
      }else{
        int width = min(gi->width, gj->width);
        for(int vi=0; vi<gi->V.size(); vi++){
          for(int vj=0; vj<gj->V.size(); vj++){
            Node *va = gi->V[vi];
            Node *vb = gj->V[vj];

            double likeness = kernel->compute(va, vb);
            if(likeness == 0) continue;
            double weight = 0;
            for(int j=0; j<width; j++){
              for(int i=0; i<=j; i++){
                double a = gi->M[vi][i][j];
                if(a==0) continue;
                double b = gj->M[vj][i][j];
                weight += a * b;
              }
            }
            sum += weight * likeness;
          }
        }
      }
      K[gii][gji] = sum;
      _calculated += 1;
      K[gji][gii] = sum;
      if(_calculated % _reportEveryN == 0 || _calculated == _kSize){
        #ifdef MEX
        //mexPrintf("K computed: %i / %i\n", _calculated, _kSize);
        //mexEvalString("pause(.001);");
        #else
        cout << "K computed: " << _calculated << "/" << _kSize << "\n";
        #endif
      }
    }
  }
}

void ThreadedLoops::printComputed(){
  int i = 0;
  while(!_complete){
    int c = _calculated;
    if(i < c){
      mexPrintf("K computed: %i / %i\n", c, _kSize);
      mexEvalString("pause(.001);");
      i = c;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  mexPrintf("K computed: %i / %i (Completed) \n", _kSize, _kSize);
}

void ThreadedLoops::computeK(double **K, vector<Graph*> *pGraphs,
                             NodeKernel *kernel){
  _nGraphs = (int) pGraphs->size();
  _kSize = (_nGraphs * _nGraphs) / 2;
  _reportEveryN = max(_kSize / 100, 1);

  _complete = false;
  // thread syncPrint = thread(&ThreadedLoops::printComputed, this);

  int chunkSize = (int) ceil(_nGraphs / _nThreads);
  int from = 0;
  vector<thread*> threads;
  for(int i=0; i<_nThreads; i++){
    int to = min(from + chunkSize, _nGraphs - 1);
    thread *t = new thread(&ThreadedLoops::computeInterval, this,
                           K, pGraphs, kernel, from, to);
    from = to + 1;
    threads.push_back(t);
  }

  for(int i=0; i<_nThreads; i++){
    threads[i]->join();
  }

  _complete = true;
  //  syncPrint.join();
}
