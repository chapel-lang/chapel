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
  // unmanaged -> unmanaged 
  if i == 2 {
    var c: C = new unmanaged D(int);
    var d = c: unmanaged D;
  }
}
