// Fake data, a stopgap measure to implementing domains and arrays

use _chpl_seq;

class _fdata : value {
  type elt_type;

  var x0 : elt_type;
  var x1 : elt_type;
  var x2 : elt_type;
  var x3 : elt_type;
  var x4 : elt_type;
  var x5 : elt_type;
  var x6 : elt_type;
  var x7 : elt_type;
  var x8 : elt_type;
  var x9 : elt_type;
  var x10 : elt_type;
  var x11 : elt_type;
  var x12 : elt_type;
  var x13 : elt_type;
  var x14 : elt_type;
  var x15 : elt_type;

  function this(i : integer) var : elt_type {
    select i {
      when 0 do return x0;
      when 1 do return x1;
      when 2 do return x2;
      when 3 do return x3;
      when 4 do return x4;
      when 5 do return x5;
      when 6 do return x6;
      when 7 do return x7;
      when 8 do return x8;
      when 9 do return x9;
      when 10 do return x10;
      when 11 do return x11;
      when 12 do return x12;
      when 13 do return x13;
      when 14 do return x14;
      when 15 do return x15;
    }
    halt("data access out-of-bounds error");
  }
}

function write(val : _fdata) {
  write("[|");
  for i in 0..15 {
    write(val(i));
    write("|");
  }
  write("]");
}
