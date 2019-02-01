extern {
  struct A_struct {
    int i;
    const struct B_struct* b;
  };
  
  struct B_struct {
    struct A_struct* a;
  };
  
  static struct B_struct* make(void) { return 0;}
}


var b1 = make();
var b2 : c_ptr(B_struct) = make();
writeln(b1.type:string);
writeln(b2.type:string);
