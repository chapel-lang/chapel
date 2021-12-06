proc foo(a:int, b=a*2) throws {
  return a+b;
}
var x = try! foo(1);

