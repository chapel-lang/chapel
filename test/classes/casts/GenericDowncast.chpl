class C {}
class D : C { type T; }

config param i = 0;

proc main() {
  // owned -> owned
  if i == 0 {
    var c: C = new D(int);
    var d = c: owned D;
  }
  // shared -> shared
  if i == 1 {
    var c: C = new shared D(int);
    var d = c: shared D;
  }
  // borrowed -> borrowed
  if i == 2 {
    var c: C = new borrowed D(int);
    var d = c: borrowed D;
  }
  // unmanaged -> unmanaged 
  if i == 3 {
    var c: C = new unmanaged D(int);
    var d = c: unmanaged D;
  }
}
