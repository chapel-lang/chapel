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

var a = (new owned Cons(1, nil)).borrow();
var aa = (new owned Cons(2, a)).borrow();

var b = (new owned Cons(1.0, nil)).borrow();
var bb = (new owned Cons(2.0, b)).borrow();


print(aa);
print(bb);
