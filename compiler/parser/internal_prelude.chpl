class Index {
}

class Array {
  function self(i) {
    return __primitive("array_index", self, i);
  }
  function self(i; j) {
    return __primitive("array_index", self, i, j);
  }
}

class Domain {
}

class Tuple {
}

function malloc(size: integer): {
}
