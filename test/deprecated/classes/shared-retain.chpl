// deprecated in 1.31 by jade
class A {}

{
  var a = new unmanaged A();
  var s: shared A? = new shared A();
  s.retain(a);
}
