class Cons {
  var car;
  var cdr; 
};

fun print(c : Cons) {
  if (c != nil) {
     writeln(c.car);
     print(c.cdr);
  }
}

var a = Cons(1, nil);
var aa = Cons(2, a);

var b = Cons(1.0, nil);
var bb = Cons(2.0, b);


print(aa);
print(bb);
