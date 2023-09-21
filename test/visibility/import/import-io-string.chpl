import IO.string;

proc foo() {
	var a: string = "%06xi";
	return a.format(1);
}
writeln(foo());
