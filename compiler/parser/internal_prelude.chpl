class Index {
}

class Array {
  function this(i) {
    return __primitive("array_index", this, i);
  }
  function this(i; j) {
    return __primitive("array_index", this, i, j);
  }
}

class Domain {
}

class Tuple {
}

function malloc(size: integer) {
}

function _UNION_CHECK(u, tag, filename, lineno) { }
function _UNION_SET(u, tag) { }
