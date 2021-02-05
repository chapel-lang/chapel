interface LessThan(type T) {
	proc LT(x:T, y:T):bool;
}

proc LT(x:int, y:int):bool {
	return x<y;
}

implements LessThan(int);

proc DoLT(x:?T, y:T):T where implements LessThan(T) {  
  if(LT(x,y)) {
    return x;
  } else {
    return y;
  }
}

proc minFn(x:?T,y :T):T where implements LessThan(T) {
  return DoLT(x, y);
}

writeln(minFn(8,1));
