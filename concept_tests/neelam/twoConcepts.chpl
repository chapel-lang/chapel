interface EqualityComparable {
	proc ==(x : self, y: self): bool;
}

interface LessThanComparable {
	proc <(x : self, y: self):bool;
}
