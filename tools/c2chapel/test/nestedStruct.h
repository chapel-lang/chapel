struct Outer {
  struct first {
    int a;
    char* b;
  } structField;

  struct second {
    int* a;
    int* b;
  } *fieldPtr;
};
