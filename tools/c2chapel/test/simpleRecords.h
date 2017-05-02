
// Simple records that Chapel should be able to generate

struct allInts {
  int a;
  unsigned int b;
  long long c;
};

struct misc {
  char a;
  char* b;
  void* c;
  int* d;
};

struct composition {
  struct misc m;
  struct allInts* i;
};

void structArgs(struct misc a, struct misc b);

struct allInts retStruct(struct misc a);

void structPointer(struct misc* a);
