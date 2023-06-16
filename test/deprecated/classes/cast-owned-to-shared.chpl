// deprecated by Jade in 1.31: only tests the versions that do not have `compilerError`
class A {}

{
  var c = new owned A();
  var result = c:shared A;
}
{
  var c = new owned A?();
  var result = c:shared A?;
}
