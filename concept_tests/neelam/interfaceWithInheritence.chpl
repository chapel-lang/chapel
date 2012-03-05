interface EqualityComparable {
	proc ==(x:self, y:self):bool;
}

interface LessThanComparable : EqualityComparable {
	proc <(x:self,y:self):bool;
}
