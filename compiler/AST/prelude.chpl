-- I/O

function write() {}
function writeln() {}


-- intrinsic types

enum boolean = true | false;



-- math

function sqrt(x: float): float {}
function abs(x:?t): float {}  -- BLC: should be ": t"



-- boundary classes/functions

function wrap() {}


-- reductions

class reduction {
}

class sum : reduction {
}

class max : reduction {
}

class min : reduction {
}

class maxloc : reduction {
}

class minloc : reduction {
}
