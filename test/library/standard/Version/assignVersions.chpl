use Version;

var zero = 0;
var one = 1;
var two = 2;
var three = 3;
var maxVal = 999;

var ret = new version(maxVal,maxVal,maxVal);
var v0_0_0 = new version(zero,zero,zero);
var v0_0_1 = new version(zero,zero,one);

if v0_0_0 < v0_0_1 {
  ret = v0_0_1;
}
writeln(ret);

var retp = new versionValue(max(int), max(int), max(int));
var v0_0_0p = new versionValue(0,0,0);
var v0_0_1p = new versionValue(0,0,1);

proc getVersion(otherVer: versionValue) {
  var ret = new version(-1,-1,-1);
  if (true) {
    ret = otherVer;
  }
  return ret;
}

writeln(getVersion(v0_0_1p):string);
// if v0_0_0p < v0_0_1p {
//   this doesn't work because the fields are params and we can't reassign them
//   retp = v0_0_1p;
// }
// writeln(retp);
