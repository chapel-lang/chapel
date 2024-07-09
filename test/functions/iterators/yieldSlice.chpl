iter iterator() {
	var arr : [1..10] int;
	yield arr[1..5];
}

for x in iterator() {
	writeln(x, ": ", x.type:string);
}
