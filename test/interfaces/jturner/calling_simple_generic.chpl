interface LessThan {
	proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int):bool {
	return x<y;
}

int implements LessThan;

proc DoID(x:?T):T { 
    return x; 
}

proc minFn(x:?T,y :T):T where T implements LessThan {
  return DoID(x);
}

writeln(minFn(3,4));
