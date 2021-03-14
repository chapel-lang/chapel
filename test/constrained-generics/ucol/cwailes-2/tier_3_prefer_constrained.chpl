interface LessThan(T) {
	proc LT(x:T,y:T):bool;
}

implements LessThan(int);

proc LT(x:int, y:int):bool {
	return x < y;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
	if (LT(x,y)) {
		return x;
	} else {
		return y;
	}
}

proc minFn(x:?T, y:T):T {
	if (LT(x,y)) {
		return x*x;
	} else {
		return y*y;
	}
}

// Returns 3
writeln(minFn(3,4));
