use List;

var lst1: list(int, false);
var lst2: list(int, true);

if lst1.parSafe == lst2.parSafe then
  compilerError("Lists have matching parSafe values");
