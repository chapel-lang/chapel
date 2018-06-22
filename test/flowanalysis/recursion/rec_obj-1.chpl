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

var a = new borrowed Cons(1, nil);
var aa = new borrowed Cons(2, a);

var b = new borrowed Cons(1.0, nil);
var bb = new borrowed Cons(2.0, b);


print(aa);
print(bb);
