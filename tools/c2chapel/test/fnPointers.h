typedef struct {
  int (*close)(int foo);
  int (*open)(char bar);
} io_methods;

typedef int (*myFunctionPointer)(void*, int, char*);

int fnPointerArgs(int (*callback)(int, int), char* msg);

int foo(myFunctionPointer a, int b);
