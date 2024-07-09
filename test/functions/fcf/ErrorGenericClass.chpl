record R {
	var x : RootClass; // generic 'lifetime'
}

var x = proc(r : R, x: RootClass) : void { };
compilerWarning(x.type : string);
