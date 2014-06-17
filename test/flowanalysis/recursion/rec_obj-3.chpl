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

proc reverse_internal(c, e) {
  if (c != nil) then
    return reverse_internal(c.cdr, new Cons(c.car, e));
  else
    return e;
}

proc reverse(c : Cons) { return reverse_internal(c, nil); }

var a = new Cons(1, new Cons(2, new Cons(3, nil)));
var b = new Cons(1.0, new Cons(2.0, new Cons(3.0, nil)));

print(reverse(a));
print(reverse(b));
