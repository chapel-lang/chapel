use _chpl_seq;
use _chpl_data;

class _htuple : value {
  type elt_type;
  param size : integer;
  var elements : _fdata(elt_type); -- should be size
  function this(i : integer) var : elt_type {
    if i < 1 or i > size then
      halt("tuple indexing out-of-bounds error");
    return elements(i-1);
  }
}

function write(val : _htuple) {
  write("(");
  for i in 1..val.size-1 {
    write(val(i));
    write(", ");
  }
  write(val(val.size));
  write(")");
}
