interface LessThan {
	proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int):bool {
	return x<y;
}

int implements LessThan;

proc DoLT(x:?T, y:T):bool where T implements LessThan {  
    return LT(x, y);
}

proc minFn(x:?T,y :T):T where T implements LessThan {
	if(DoLT(x,y)) {
		return x;
	}
	else {
		return y;
	}
}

writeln(minFn(3,4));
