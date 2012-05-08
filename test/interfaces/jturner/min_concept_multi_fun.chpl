interface LessThan {
	proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int):bool {
	return x<y;
}

int implements LessThan;

proc ID(x:?T, y:T) where T implements LessThan {  
    return LT(x, y);
}

proc minFn(x:?T,y :T) where T implements LessThan {
	if(ID(x,y)) {
		return x;
	}
	else {
		return y;
	}
}

writeln(minFn(3,4));
