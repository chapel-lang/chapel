record MyArray {
  var D: domain(1);
  var A: [D] int;
}

{
  var x:MyArray;
  var y:MyArray;

  x = y;
}

