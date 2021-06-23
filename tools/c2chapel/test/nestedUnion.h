union Outer {
  union first {
    int a;
    char* b;
  } unionField;

  union second {
    int* a;
    int* b;
  } *fieldPtr;
};
