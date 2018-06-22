class Cons {
  var car;
  var cdr; 
};

proc print(c : borrowed Cons) {
  if (c != nil) {
     writeln(c.car);
     print(c.cdr);
  }
}

var a = new borrowed Cons(1, new borrowed Cons(2, nil));
var b = new borrowed Cons(1.0, new borrowed Cons(2.0, nil));

print(a);
print(b);
