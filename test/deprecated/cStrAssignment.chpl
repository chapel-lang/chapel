config const errMode = 0;

// UTF-8 case
{
  var cStr = c"my C string";

  // copy initializer
  { 
    var s: string = cStr;
    writeln(s);
    writeln(s.numCodepoints);  // make sure this runs correctly
  }

  // = overload
  {
    var s = "my Chapel string";
    s = cStr;
    writeln(s);
    writeln(s.numCodepoints);  // make sure this runs correctly
  }
}

// UTF-8 case
{
  var cStr = c"\xff";

  // copy initializer
  if errMode == 1 { 
    var s: string = cStr;  // should halt
    writeln(s);
    writeln(s.numCodepoints);
  }

  // = overload
  if errMode == 2 {
    var s = "my Chapel string";
    s = cStr;  // should halt
    writeln(s);
    writeln(s.numCodepoints);
  }
}
