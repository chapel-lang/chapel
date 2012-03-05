interface Stack(X) {
	type itemType;
	proc X.push(x:itemType);
	proc X.pop():itemType;
	proc X.isEmpty():bool;
}
