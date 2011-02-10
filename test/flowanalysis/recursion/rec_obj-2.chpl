class Cons {
  var car;
  var cdr; 
};

proc print(c : Cons) {
  if (c != nil) {
     writeln(c.car);
     print(c.cdr);
  }
}

var a = new Cons(1, new Cons(2, nil));
var b = new Cons(1.0, new Cons(2.0, nil));

print(a);
print(b);
