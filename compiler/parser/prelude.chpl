-- I/O

function write() {}
function writeln() {}
function read() {}


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

class sum {
}

class max {
}

class min {
}

class maxloc {
}

class minloc {
}


-- timers

class timer {
  function start();
  function stop();
  function read(): float;
}



function _UNION_CHECK(u, tag, filename, lineno) { }
function _UNION_SET(u, tag) { }
