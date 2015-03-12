var D:domain(1);
var A:[D] int;


on Locales[1] {
  var D1:domain(1);
  var A1:[D1] int;

  A1 = A;
}

on Locales[1] {
  var D1:domain(1);
  var A1:[D1] int;

  A = A1;
}

on Locales[1] {
  var D1:domain(1);
  var A1:[D1] int;

  on Locales[2] {
    A = A1;
  }
}

