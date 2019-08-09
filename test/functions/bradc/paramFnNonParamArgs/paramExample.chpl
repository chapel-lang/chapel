config type xtype = real;
config type ytype = int;

proc widthOfResult(x, y) param {
  var z = x + y;
  return numBits(z.type);
}

var x = read(xtype);
var y = read(ytype);

param width = widthOfResult(x, y);
if (width > 32) then
  compilerWarning("width of x + y is " + width:string);

writeln("x + y is: ", x+y);
