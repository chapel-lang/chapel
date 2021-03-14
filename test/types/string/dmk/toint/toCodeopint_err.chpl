// Make good and sure we build a non-param string.
var a: string = codepointToString(65);
var b: string = codepointToString(66);
var s: string = a + b;

writeln(s.toCodepoint());
