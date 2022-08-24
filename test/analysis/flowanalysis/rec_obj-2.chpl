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

var a = (new owned Cons(1, (new owned Cons(2, nil)).borrow())).borrow();
var b = (new owned Cons(1.0, new borrowed Cons(2.0, nil))).borrow();

print(a);
print(b);
