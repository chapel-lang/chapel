interface Stack{
	type itemType;
	proc self.push(x: itemType);
	proc self.pop(): itemType;
	proc self.isEmpty():bool;
}
