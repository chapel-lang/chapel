interface LessThan(T) {
	proc LT(x:T, y:T):bool;
}

proc LT(x:int, y:int):bool {
	return x<y;
}

implements LessThan(int);

proc DoLT(x:?T, y:T):bool where implements LessThan(T) {  
    return LT(x, y);
}

proc minFn(x:?T,y :T):T where implements LessThan(T) {
	if(DoLT(x,y)) {
		return x;
	}
	else {
		return y;
	}
}

writeln(minFn(3,4));
