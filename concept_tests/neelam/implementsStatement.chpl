interface Semigroup {
proc binary_op(x : self, y : self):self;
}
interface Monoid : Semigroup {
proc identity_elt(): self;
}
int implements Semigroup {
proc binary_op(x : int, y : int):int { return x + y; }
}
int implements Monoid {
proc identity_elt(x : int, y : int):int { return 0; }
}
