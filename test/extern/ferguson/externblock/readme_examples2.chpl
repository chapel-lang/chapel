module C {
  extern {
    static int foo(int x) { return x + 1; }
  }
}

writeln(C.foo(3));


extern {
  #define NEG_ONE (-1)
  #define MY_NUMBER (NEG_ONE)
}
writeln(MY_NUMBER);


extern {
  #define ADD(x,y) ((x)+(y))
  static inline int ADD_int_int(int x, int y) { return ADD(x,y); }
}
var sum = ADD_int_int(1,2);
writeln(sum);


extern {
  static void setItToOne(int* x) { *x = 1; }
  // will translate automatically into
  //  extern proc setItToOne(x:c_ptr(c_int));

  // The Chapel compiler can't know if X is used as an array,
  // if the argument will come from a Chapel variable, and in more general
  // cases, how to handle multiple levels of pointers. For example:
  static void setSpace(int** x) {
    static int space[10];
    *x = space;
  }
  // translates automatically into
  //  extern proc returnSpace( x:c_ptr(c_ptr(c_int)) );

  static void setString(const char** x) { *x = "My String"; }
}
var x:c_int;
setItToOne(c_ptrTo(x));
writeln(x);

var space:c_ptr(c_int);
setSpace(c_ptrTo(space));

var str:c_string;
setString(c_ptrTo(str));
writeln(str:string);

module MyCModule {
  extern {
    static int bar(int b) { return b + 1; }
  }
}

writeln("Hello");
writeln(MyCModule.bar(7));

