//struct typedef'd to same name
typedef struct td_struct {
  double d;
} td_struct;

//typedef'd to different name
typedef struct td_struct2 {
  int x;
} y;

//anonymous struct
typedef struct {
  const char* c;
  int foo;
} bar;
