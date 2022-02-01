use IO;

var a = stringStyleWithLengthInternal(0);
var b = stringStyleWithLengthInternal(1);
var c = stringStyleWithLengthInternal(2);
var d = stringStyleWithLengthInternal(4);
var e = stringStyleWithLengthInternal(8);

// Those should have worked

// This should throw an error
stringStyleWithLengthInternal(100);
