
var total : int;

cobegin with (+ reduce total) {
  on Locales[0] do
    total = 2;
  on Locales[0] do
    total = 3;
}

writeln("total ", total);

// 'begin' and 'on' are on the same line so that the line number
// in the error message is the same for comm=none and gasnet,
// for matching against .good.
//
begin with (+ reduce total) {  on Locales[0] do  total = 7;  }

writeln("total ", total);
