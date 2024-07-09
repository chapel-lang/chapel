class Cons {
  var car;
  var cdr; 
};

proc print(c : borrowed Cons(?)) {
  if (c != nil) {
     writeln(c.car);
     print(c.cdr);
  }
}

var a = new Cons(1, nil);
var aa = new Cons(2, a.borrow());

var b = new Cons(1.0, nil);
var bb = new Cons(2.0, b.borrow());

print(aa);
print(bb);
