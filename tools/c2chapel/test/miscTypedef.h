
typedef int my_int;

typedef char* my_string;

struct simpleStruct {
  int a;
  char b;
  void* c;
  my_int d;
  my_string e;
};

typedef struct simpleStruct renamedStruct;

typedef struct {
  int a;
  int b;
  renamedStruct c;
  struct simpleStruct d;
} fancyStruct;

typedef fancyStruct renamedFancy;

fancyStruct retStruct(my_int a, my_int b, renamedStruct r);

void tdPointer(fancyStruct* a, renamedStruct** b);

typedef struct {
  int i;
  char c;
} *recp;

struct empty_struct;

struct forwardStruct;
typedef struct forwardStruct fwdStruct;

struct forwardStruct {
  int a;
  int b;
};

