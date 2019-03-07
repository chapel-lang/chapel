typedef void (*func) (void*);

typedef struct MyRec_p {
  func x;
} MyRec;

MyRec getMyRec(void);
void takeFuncPtr(func* arg);
