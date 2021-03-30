
typedef int my_int;

typedef char* my_string;

union simpleUnion {
  int a;
  char b;
  void* c;
  my_int d;
  my_string e;
};

typedef union simpleUnion renamedUnion;

typedef union {
  int a;
  int b;
  renamedUnion c;
  union simpleUnion d;
} fancyUnion;

typedef fancyUnion renamedFancy;

fancyUnion retUnion(my_int a, my_int b, renamedUnion r);

void tdPointer(fancyUnion* a, renamedUnion** b);

typedef union {
  int i;
  char c;
} *recp;

union empty_union;

union forwardUnion;
typedef union forwardUnion fwdUnion;

union forwardUnion {
  int a;
  int b;
};

