extern {
  typedef enum {
    ENUM_A = 1,
    ENUM_B = 0,
    ENUM_C = -1
  } MyEnum;
}

var x:MyEnum = ENUM_A;


writeln(x);
