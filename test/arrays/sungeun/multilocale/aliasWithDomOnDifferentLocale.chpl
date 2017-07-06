on Locales(numLocales-1) {
  var D = {1..6}; 
  var D2 = {2..5}; 
  on Locales(0) {
    var A: [D] int;
    A = -1;
    ref Aa = A[D2];
    Aa = 1;
    writeln(A);
  }
}

on Locales(0) {
  var D = {1..6}; 
  var D2 = {2..5}; 
  on Locales(numLocales-1) {
    var A: [D] int;
    A = -1;
    ref Aa = A[D2];
    Aa = 1;
    writeln(A);
  }
}

{
  var D = {1..6}; 
  var D2 = {2..5}; 
  on Locales(numLocales-1) {
    var A: [D] int;
    A = -1;
    ref Aa = A[D2];
    Aa = 1;
    writeln(A);
  }
}

var D = {1..6}; 
on Locales(numLocales-1) {
  var D2 = {2..5}; 
  var A: [D] int;
  A = -1;
  ref Aa = A[D2];
  Aa = 1;
  writeln(A);
}

var D2 = {2..5}; 
on Locales(numLocales-1) {
  var D = {1..6}; 
  var A: [D] int;
  A = -1;
  ref Aa = A[D2];
  Aa = 1;
  writeln(A);
}


