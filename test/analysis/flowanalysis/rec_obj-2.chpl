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

ref a = (new Cons(1, (new Cons(2, nil)).borrow())).borrow();
ref b = (new Cons(1.0, (new Cons(2.0, nil)).borrow())).borrow();

print(a);
print(b);
