class Cons {
  var car;
  var cdr; 
};

function print(c : Cons) {
  if (c != nil) {
     writeln(c.car);
     print(c.cdr);
  }
}

var a = Cons();
a.car = 1;
a.cdr = nil;
var aa = Cons();
aa.car = 2;
aa.cdr = a;

var b = Cons();
b.car = 1.0;
b.cdr = nil;
var bb = Cons();
bb.car = 2.0;
bb.cdr = b;


print(aa);
print(bb);
