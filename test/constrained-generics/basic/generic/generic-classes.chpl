interface someInterface {}

class myClass : someInterface {
  type T;
}

proc test(x : someInterface) {}

var x = new myClass(int);
test(x);
