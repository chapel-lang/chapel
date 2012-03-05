interface Vector {
	type eltType;
	proc norm(v:self):eltType;
	proc +(u:self,v:self):self;
	proc self.size():int;
	proc self.these():eltType;
}
