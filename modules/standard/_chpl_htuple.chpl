use _chpl_seq;

record _htuple {
  type _elementType;
  param _size : integer;
--  var _elements : _data(_elementType, _size);
  var _elements0 : _elementType;
  var _elements1 : _elementType;
  var _elements2 : _elementType;
  function this(i : integer) var : _elementType{
    select i {
      when 1 do return _elements0;
      when 2 do return _elements1;
      when 3 do return _elements2;
    }
    halt("Tuple indexed out of bounds, current tuple size limited to 3");
  }
}

function write(val : _htuple) {
  write("(");
  var first = true;
  for i in 1..val._size {
    if not first {
      write(", ");
    }
    write(val(i));
    first = false;
  }
  write(")");
}
