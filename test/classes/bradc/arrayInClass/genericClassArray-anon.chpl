class C {
  type t;

  var x: t;

  proc foo() {
    writeln("x.domain is: ", x.domain);
    writeln("x is: ", x);
    writeln();
  }
}

var arithC = new unmanaged C([1..3] real);
[i in 1..3] arithC.x(i) = i + i/10.0;
arithC.foo();
delete arithC;

/* Should the following be legal?
enum probClass {S, W, A, B, C};
var enumC = new C([domain(enumDom)] real);
[i in enumDom] enumC.x(i) = i:int + i:real/10.0;
enumC.foo();
*/
