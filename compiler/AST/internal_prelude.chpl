
-- Here is a function which I can use to call a primitive if an array is
-- used as a function and applied to an Index (tuple)

class _Array {
  function self(i: index) {
    return __primitive("array_index", i);
  }
}

-- the idea of this function is that it gets invoked when
-- var A : [D] 1.0;
-- A(1); -- HERE
-- the "self" is just a way of saying that it is not a normal
-- member function which would get invoked as A.XXX(1), but
-- instead is called when the object is used as a function.
