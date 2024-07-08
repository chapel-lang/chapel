// this test contains examples from issue #17756
{
  var x: uint(8) = 1;
  var y: uint(64) = 2;
  writeln(x+y);
}

{
  var x: uint(64);
  var y: int(64);

  var z = min(x,y);
  writeln(z, " : ", z.type:string);
}
