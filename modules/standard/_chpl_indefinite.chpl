var _ps : _ddata(int) = _ddata(int, 27);
_ps.init();

_ps(0) = 23;
_ps(1) = 53;
_ps(2) = 97;
_ps(3) = 193;
_ps(4) = 389;
_ps(5) = 769;
_ps(6) = 1543;
_ps(7) = 3079;
_ps(8) = 6151;
_ps(9) = 12289;
_ps(10) = 24593;
_ps(11) = 49157;
_ps(12) = 98317;
_ps(13) = 196613;
_ps(14) = 393241;
_ps(15) = 786433;
_ps(16) = 1572869;
_ps(17) = 3145739;
_ps(18) = 6291469;
_ps(19) = 12582917;
_ps(20) = 25165843;
_ps(21) = 50331653;
_ps(22) = 100663319;
_ps(23) = 201326611;
_ps(24) = 402653189;
_ps(25) = 805306457;
_ps(26) = 1610612741;

class _idomain : _domain {
  type ind_type;
  var num_inds : int;
  var size : int = 0;
  var table : _ddata(int) = _ddata(int, _ps(size));
  var inds : _ddata(ind_type) = _ddata(ind_type, _ps(size)/2);

  fun initialize() {
    table.init();
    inds.init();
  }

  iterator _forall() : ind_type {
    forall i in 0..num_inds-1 do
      yield inds(i);
  }

  fun getHeadCursor()
    return 0;

  fun getNextCursor(c)
    return c + 1;

  fun getValue(c)
    return inds(c);

  fun isValidCursor?(c)
    return c < num_inds;

  fun _double() {
    size = size + 1;
    var inds_copy = _ddata(ind_type, _ps(size)/2);
    inds_copy.init();
    for i in 0.._ps(size-1)/2-1 do
      inds_copy(i) = inds(i);
    inds = inds_copy;

    table = _ddata(int, _ps(size));
    table.init();
    for i in 0.._ps(size-1)-1 do
      table(_map(inds(i))) = i+1;
  }

  fun _map(ind : ind_type) : int {
    var probe = 0;
    while true {
      var i = (_indefinite_hash(ind) + probe**2) % _ps(size);
      if table(i) == 0 then
        return i;
      if inds(table(i)-1) == ind then
        return i;
      probe = probe + 1;
    }
    return -1;
  }

  fun _get_index(ind : ind_type)
    return table(_map(ind)) - 1;

  fun add(ind : ind_type) {
    if (table(_map(ind)) == 0) {
      num_inds = num_inds + 1;
      if num_inds == _ps(size)/2 then
        _double();
      table(_map(ind)) = num_inds;
      inds(num_inds-1) = ind;

    }
  }

  fun _build_array(type elt_type)
    return _iarray(elt_type, ind_type, dom=this);
}

class _iarray : value {
  type elt_type;
  type ind_type;
  var dom : _idomain(ind_type);
  var data : _ddata(elt_type) = _ddata(elt_type, 128);

  fun initialize() {
    data.init();
  }

  fun this(ind : ind_type) var : elt_type
    return data(dom._get_index(ind));

  fun getHeadCursor()
    return 0;

  fun getNextCursor(c)
    return c + 1;

  fun getValue(c)
    return data(c);

  fun isValidCursor?(c)
    return c < dom.num_inds;
}

fun fwrite(f : file, x : _idomain) {
  fwrite(f, "[", x.inds(0));
  for i in 1..x.num_inds-1 do
    fwrite(f, ", ", x.inds(i));
  fwrite(f, "]");
}

fun fwrite(f : file, x : _iarray) {
  fwrite(f, x.data(0));
  for i in 1..x.dom.num_inds-1 do
    fwrite(f, " ", x.data(i));
}

fun _indefinite_hash(x : int)
  return x;

fun _indefinite_hash(x : string)
  return 1;
