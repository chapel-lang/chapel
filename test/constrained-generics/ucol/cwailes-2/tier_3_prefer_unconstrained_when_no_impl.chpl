interface LessThan(type T) {
	proc LT(x:T,y:T):bool;
}

proc LT(x:int, y:int) : bool {
	return x < y;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
	if (LT(x,y)) {
		return x;
	}	else {
		return y;
	}
}

proc minFn(x:?T, y:T) : T {
	if (LT(x, y)) {
		return x*x;
	}	else {
		return y*y;
	}
}

// Writes 9
writeln(minFn(3,4));
