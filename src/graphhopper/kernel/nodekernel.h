#ifndef NODE_KERNEL_H
#define NODE_KERNEL_H



class NodeKernel{

 public:
  virtual double compute(Node *v1, Node *v2) = 0;


};



#endif
