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

proc reverse_internal(c, e) {
  if (c != nil) then
    return reverse_internal(c.cdr, new unmanaged Cons(c.car, e));
  else
    return e;
}

proc reverse(c : unmanaged Cons) { return reverse_internal(c, nil); }

var a = new unmanaged Cons(1, new unmanaged Cons(2, new unmanaged Cons(3, nil)));
var b = new unmanaged Cons(1.0, new unmanaged Cons(2.0, new unmanaged Cons(3.0, nil)));

print(reverse(a));
print(reverse(b));
