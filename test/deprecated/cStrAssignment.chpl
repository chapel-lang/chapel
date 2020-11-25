var cStr = c"my C string";

// copy initializer
{ 
  var s: string = cStr;
  writeln(s);
}

// = overload
{
  var s = "my Chapel string";
  s = cStr;
  writeln(s);
}
