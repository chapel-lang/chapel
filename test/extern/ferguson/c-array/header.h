typedef int mytriple[3];

static mytriple myglobal;

static void setpair(mytriple arg) {
  arg[0] = 0;
  arg[1] = 1;
  arg[2] = 2;
}

typedef struct {
  double dat[2];
} mycomplex;

static mycomplex myglobalcomplex;

static void setcomplex(mycomplex* arg) {
  arg->dat[0] = 0.0;
  arg->dat[1] = 10.0;
}
static mycomplex returncomplex(void) {
  mycomplex x;
  x.dat[0] = 0.0;
  x.dat[1] = 100.0;
  return x;
}
