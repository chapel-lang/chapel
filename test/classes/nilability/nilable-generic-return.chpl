class C {}

proc f(): C? {
  var x = true;
  if x then return nil;

  return new C();
}
f();
