-- I/O

function write() {}
function writeln() {}


-- intrinsic types

const false: boolean = 0;
const true: boolean = 1;


-- math

function sqrt(x: float): float {}
function abs(x: ?t): float {}  -- BLC: should be ": t"



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
