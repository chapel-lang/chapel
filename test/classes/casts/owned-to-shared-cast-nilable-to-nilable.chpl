// deprecated by Jade in 1.31, once the feature is removed these tests can be removed
class MyClass {  var x: int;  }
{
  var c = new owned MyClass?();

  var result = c:shared MyClass?;
  writeln(result, " : ", result.type:string);
}
