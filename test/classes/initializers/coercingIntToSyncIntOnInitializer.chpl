// Modified from
// test/classes/hannah/coercingIntToSyncIntOnConstructor.chpl
class Foo {
	var x$: sync int;
}


proc main() {
	var foo1 : Foo = new Foo(1);
	writeln("a = ", foo1.x$);

	var foo2 : Foo = new Foo(x$=2);
	writeln("b = ", foo2.x$);
}
