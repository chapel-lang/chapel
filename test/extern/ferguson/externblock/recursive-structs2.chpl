extern {
  typedef struct A_struct A;
  typedef struct B_struct B;
  
  struct A_struct {
    int i;
    const B* b;
  };
  
  struct B_struct {
    A* a;
  };
  
  static B* make(void) {return 0;}
}

var b1 = make();
var b2 : c_ptr(B_struct) = make();
writeln(b1.type:string);
writeln(b2.type:string);
