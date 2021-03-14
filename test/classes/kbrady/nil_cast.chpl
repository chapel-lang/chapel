class A {
}
class B : A {
}

var a:borrowed A?;
writeln("a: ",a);
var b = a:borrowed B?;
writeln("b: ",b);

var aa:unmanaged A?;
writeln("aa: ",aa);
var bb = aa:unmanaged B?;
writeln("bb: ",bb);
