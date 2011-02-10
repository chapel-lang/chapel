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

var a = new Cons(1, nil);
var aa = new Cons(2, a);

var b = new Cons(1.0, nil);
var bb = new Cons(2.0, b);


print(aa);
print(bb);
