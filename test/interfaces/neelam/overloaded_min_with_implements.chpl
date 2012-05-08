interface LessThan {
	proc LT(x:self,y:self):bool;
}

int implements LessThan;

proc LT(x:int, y:int) : bool {
	return x < y;
}

proc minFn(x:?T, y:T):T where T implements LessThan {
	if(LT(x,y)) {
		return x;
	}
	else {
		return y;
	}
}

proc minFn(x:?T, y:T) : T {
	if(x < y) {
		return x*x;
	}
	else {
		return y*y;
	}
}

writeln(minFn(3,4));
