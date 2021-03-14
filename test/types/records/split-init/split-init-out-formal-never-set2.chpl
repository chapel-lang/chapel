proc outProc(out outarg) {
  outarg = outarg + 100;
}

var inVar: int = 1;
var outVar: int = 2;
var inoutVar: int = 3;
var refVar: int = 4;
outProc(outVar);
