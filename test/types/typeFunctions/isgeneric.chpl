record ConcreteR { var x:int; }
record GenericR { var x; }
record ConcreteC { var x:int; }
record GenericC { var x; }


assert(!isGeneric(int));
assert(!isGenericType(int));

assert(isGeneric(GenericR));
assert(isGenericType(GenericR));
assert(!isGeneric(GenericR(int)));
assert(!isGenericType(GenericR(int)));

assert(!isGeneric(ConcreteR));
assert(!isGenericType(ConcreteR));

assert(isGeneric(GenericC));
assert(isGenericType(GenericC));
assert(!isGeneric(GenericC(string)));
assert(!isGenericType(GenericC(string)));

assert(!isGeneric(ConcreteC));
assert(!isGenericType(ConcreteC));
