
  enum color {red=1, green, blue};
  enum color2 {red, green, blue=3};
  enum color3 {red, green, blue};

  config param case = 6;
  config var badcolor = "reed";
  config var badint = 7;
  config var badint2 = 1;
  config var badcolor2 = color2.red;

  if case == 1 {  // cast param string to enum
    var x = "reed": color;
    writeln(x);
  }
  if case == 2 {  // cast non-param string to enum
    var x = badcolor: color;
    writeln(x);
  }
  if case == 3 {  // cast param int to enum
    var x = 7: color;
    writeln(x);
  }
  if case == 4 {  // cast non-param int to enum
    var x = badint: color;
    writeln(x);
  }
  if case == 5 {  // cast param enum to int
    var x = color2.red: int;
    writeln(x);
  }
  if case == 6 {  // cast non-param enum to int
    var x = badcolor2: int;
    writeln(x);
  }
  if case == 7 {  // cast param int to enum w/out that int
    var x = 1: color2;
    writeln(x);
  }
  if case == 8 {  // cast non-param int to enum w/out that int
    var x = badint2: color2;
    writeln(x);
  }
  if case == 9 {  // cast param int to enum w/out any ints
    var x = 1: color3;
    writeln(x);
  }
  if case == 10 {  // cast non-param int to enum w/out any ints
    var x = badint2: color3;
    writeln(x);
  }
