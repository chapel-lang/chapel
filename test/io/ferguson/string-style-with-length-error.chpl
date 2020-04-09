use IO;

var a = stringStyleWithLength(0);
var b = stringStyleWithLength(1);
var c = stringStyleWithLength(2);
var d = stringStyleWithLength(4);
var e = stringStyleWithLength(8);

// Those should have worked

// This should throw an error
stringStyleWithLength(100);
