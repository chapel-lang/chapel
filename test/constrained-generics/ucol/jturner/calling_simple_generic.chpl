interface LessThan(T) {
	proc LT(x:T, y:T):bool;
}

proc LT(x:int, y:int):bool {
	return x<y;
}

implements LessThan(int);

proc DoID(x:?T):T { 
    return x; 
}

proc minFn(x:?T,y :T):T where implements LessThan(T) {
  return DoID(x);
}

writeln(minFn(3,4));
