class MyClass {  var x: int;  }

{
  var c = new owned MyClass();

  var result = c:shared MyClass;
  writeln(result, " : ", result.type:string);
}
