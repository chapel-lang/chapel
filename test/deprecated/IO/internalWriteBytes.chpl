use IO;
use CTypes;

var a = [1,2,3,4] : uint(8);
var ap = c_ptrTo(a);

var f = openWriter("wb.txt");
f.writeBytes(ap, 4);
