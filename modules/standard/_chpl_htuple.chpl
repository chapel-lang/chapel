class _htuple : value {
  type elt_type;
  param size : int;
  var elements : _ddata(elt_type) = _ddata(elt_type, size);
  function initialize() {
    elements.init();
  }
  function this(i : int) var : elt_type {
    if i < 1 or i > size then
      halt("tuple indexing out-of-bounds error");
    return elements(i-1);
  }
}

function =(x : _htuple, y) {
  for i in 1..x.size {
    x(i) = y(i);
  }
  return x;
}

function fwrite(f : file, val : _htuple) {
  fwrite(f, "(");
  for i in 1..val.size-1 {
    fwrite(f, val(i));
    fwrite(f, ", ");
  }
  fwrite(f, val(val.size));
  fwrite(f, ")");
}
