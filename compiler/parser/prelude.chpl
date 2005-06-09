class pragma "codegen data" _data { -- a c array, basically
  type t;
  var x : t; -- dummy
  pragma "rename _data_this" function this(i : integer) : t {
    return __primitive("array_index", t, i);
  }
  pragma "rename _data_set" function =this(i : integer, val : t) : t {
    return __primitive("array_set", this, i, val);
  }
  pragma "rename _data_alloc" function alloc(size, description) {  }
}

-- I/O

pragma "rename _chpl_write_boolean" function write(x : boolean) : void {
         __primitive("write", x);
}
pragma "rename _chpl_write_integer" function write(x : integer) : void {
         __primitive("write", x);
}
pragma "rename _chpl_write_float" function write(x : float) : void {
         __primitive("write", x);
}
pragma "rename _chpl_write_complex" function write(x : complex) : void {
                __primitive("write", x);
}
pragma "rename _chpl_write_string" function write(x : string) : void {
         __primitive("write", x);
}
pragma "rename _chpl_write_nil" function write(x : _nilType) : void {
         __primitive("write", x);
}
pragma "rename _chpl_write_linefeed" function writeln() : void {
         __primitive("writeln");
}
pragma "rename _chpl_read_boolean" function read(inout x : boolean) : void {
         __primitive("read", x);
}
pragma "rename _chpl_read_integer" function read(inout x : integer) : void {
         __primitive("read", x);
}
pragma "rename _chpl_read_float" function read(inout x : float) : void {
         __primitive("read", x);
}
pragma "rename _chpl_read_complex" function read(inout x : complex) : void {
         __primitive("read", x);
}
pragma "rename _chpl_read_string" function read(inout x : string) : void {
         __primitive("read", x);
}

pragma "rename _chpl_tostring_boolean"
function _tostring(x : boolean, format : string) : string {
         return __primitive("coerce", string, x);
}

pragma "rename _chpl_tostring_integer"
function _tostring(x : integer, format : string) : string {
         return __primitive("coerce", string, x);
}

pragma "rename _chpl_tostring_float"
function _tostring(x : float, format : string) : string {
         return __primitive("coerce", string, x);
}

pragma "rename _chpl_tostring_complex"
function _tostring(x : complex, format : string) : string {
         return __primitive("coerce", string, x);
}

-- intrinsic type values

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

-- exits

pragma "rename _chpl_exit" function exit(status : integer);

function halt() {
  __primitive("halt");
}


-- memory tests (These tests will be moved to a module, once we have modules.)

function _chpl_memtest_printMemTable();
function _chpl_memtest_printMemStat();
function _chpl_memtest_resetMemStat();
function _chpl_memtest_allocAndFree();
function _chpl_memtest_freedMalloc();
function _chpl_memtest_freedWithoutMalloc();
function _chpl_memtest_reallocWithoutMalloc();
function _chpl_memtest_reallocZeroSize();
function _chpl_memtest_mallocOutOfMemory();
function _chpl_memtest_reallocOutOfMemory();

function _complex_read_hack(inout x) {
  __primitive("read", x);
}

function _complex_tostring_hack(x, format : string) : string {
  return __primitive("coerce", string, x);
}
