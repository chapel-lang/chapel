param _MIN_SIZE = 0;
param _MAX_SIZE = 26;

var _ps : _ddata(int) = _ddata(int, 27);
_ps.init();
_ps(_MIN_SIZE) = 23;
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
_ps(_MAX_SIZE) = 1610612741;



param _EMPTY   = -1;
param _DELETED = -2;


record _ind_data_t {
  type itype;
  var  data: itype;
  var  valid: bool;
}


class _idomain {
  type ind_type;
  var num_inds: int;
  var size: int = 0;
  var table: _ddata(int);
  var inds: _ddata(_ind_data_t(ind_type));
  var free_inds: _stack(int);
  var deleted_seen: int;
  var is_compact: bool;


  // compiler-internal routines

  def initialize() {
    table  = _ddata(int, _ps(size));
    table.init( _EMPTY);
    inds =  _ddata(_ind_data_t(ind_type), _ps(size)/2);
    inds.init();
    free_inds = _stack(int);
    deleted_seen = _EMPTY;
    is_compact = false;
  }

  def getHeadCursor() {
    return 0;
  }

  def getNextCursor(c) {
    return c + 1;
  }

  def getValue(c) {
    return inds(c).data;
  }

  def isValidCursor?(c) {
    return c < num_inds;
  }


  // internal routines

  def _half() {
    if (size > _MIN_SIZE) {
      var new_len = _ps(size-1);
      table = _ddata(int, new_len);
      table.init(_EMPTY);

      var new_inds = _ddata(_ind_data_t(ind_type), new_len/2);
      new_inds.init();
      var inds_count = 0;
      var inds_pos = 0;
      while inds_count<num_inds {
        var ind = inds(inds_pos);
        if (ind.valid) {
          new_inds(inds_count) = ind;
          table(_map(ind.data)) = inds_count;
          inds_count += 1;
        }
        inds_pos += 1;
      }
      inds = new_inds;
      size -= 1;
      free_inds.empty();
    }
  }

  def _double() {
    if (size < _MAX_SIZE) {
      var new_len = _ps(size+1);

      table = _ddata(int, new_len);
      table.init(_EMPTY);

      var new_inds = _ddata(_ind_data_t(ind_type), new_len/2);
      new_inds.init();
      var inds_count = 0;
      for inds_pos in 0..inds.size-1 {
        var ind = inds(inds_pos);
        if (ind.valid) {
          new_inds(inds_count) = ind;
          table(_map(ind.data)) = inds_count;
          inds_count += 1;
        }
      }

      inds = new_inds;
      size += 1;
      free_inds.empty();
    } else {
      halt( "cannot double idomain");
    }
  }

  def _map(ind : ind_type) : int {
    var probe = 0;
    deleted_seen = _EMPTY;
    while true {
      var i = ((_indefinite_hash(ind) + probe**2) % table.size):int(32);
      var table_i = table(i);
      if table_i==_EMPTY then
        return i;
      else if table_i==_DELETED then
        deleted_seen = i;                      
      else if inds(table_i).data==ind then
        return i;
      probe = probe + 1;
    }
    halt( "could not map index ", ind);
    return -1;  // should never get here
  }

  def _get_index(ind : ind_type) {
    var i = _map(ind);
    if (i >= 0) then
      return table(i);
    else
      halt( "index not found: ", ind);
  }

  def _build_array(type elt_type) {
    return _iarray(elt_type, ind_type, dom=this); 
  }

  def _compact() {
    if (free_inds.length > 0) {
      var inds_count = 0;
      var inds_pos = 0;
      while inds_count<num_inds {
        var ind = inds(inds_pos);
        if (ind.valid) {
          inds(inds_count) = ind;
          var i = _map(ind.data);
          if deleted_seen>0 then
            table(deleted_seen) = inds_count;
          else 
            table(i) = inds_count;
          inds_count += 1;
        }
        inds_pos += 1;
      }
    }
    free_inds.empty();
    is_compact = true;
  }

  // public routines

  def add(ind : ind_type) {
    var hash = _map(ind);
    var ind_pos = table(hash);
    if (ind_pos==_EMPTY) {
      if (deleted_seen > 0) then
        hash = deleted_seen;

      if (free_inds.length > 0) {      // reduce, reuse, recycle
        ind_pos = free_inds.pop();            
      } else {
        if (num_inds == inds.size) {
          _double();
        }
        ind_pos = num_inds;
        hash = _map(ind);
      }
      table(hash) = ind_pos;
      inds(ind_pos).data = ind;       // WAW: fixme w/ anon record literal
      inds(ind_pos).valid = true;
      num_inds += 1;
    } // else, already in domain
  }

  def remove( ind: ind_type) {
    var ind_pos = _map(ind);
    var table_i = table(ind_pos);
    if (table_i!=_EMPTY && table_i!=_DELETED) {
      is_compact = false;
      inds(table_i).valid = false;
      num_inds -= 1;
      free_inds.push( table_i);
      table(ind_pos) = _DELETED;
    }

    if (size > _MIN_SIZE) then
      if (num_inds < _ps(size-1)/2) then  // downsize?
        _half();
  }


  def member?( ind: ind_type) {
    var ind_pos = table(_map(ind));
    return (ind_pos!=_EMPTY && ind_pos!=_DELETED);
  }

  def numIndices {
    return num_inds;
  }
}

def fwrite( f : file, d : _idomain) {
  fwrite( f, "[");
  var inds_count = 0;
  var inds_pos = 0;
  while inds_count<d.num_inds {
    var ind = d.inds( inds_pos);
    if (ind.valid) {
      fwrite( f, ind.data);
      inds_count += 1;
      if (inds_count<d.num_inds) then fwrite(f, ", ");
    }
    inds_pos += 1;
  }
  fwrite(f, "]");
}


class _iarray: _abase {
  type elt_type;
  type ind_type;
  var dom : _idomain(ind_type);
  var data : _ddata(elt_type);

  def initialize() {
    data = _ddata( elt_type, _ps(0)/2);
    data.init();
  }

  def this(ind : ind_type) var : elt_type
    return data(dom._get_index(ind));

  def getHeadCursor()
    return 0;

  def getNextCursor(c)
    return c + 1;

  def getValue(c)
    return data(c);

  def isValidCursor?(c)
    return c < dom.num_inds;

  def reallocate(d: _idomain) {
    if !(d.is_compact) then d._compact();
    var new_data = _ddata( elt_type, d.inds.size);
    new_data.init();
    for i in 0..d.num_inds {
      var ind = dom.inds(dom.table(dom._map(d.inds(i).data)));
      if ind.valid {
        new_data(i) = ind.data;
      }
    }
    data = new_data;
  }

  def numElements {
    return dom.numIndices;
  }
}

def fwrite(f : file, ia : _iarray) {
  var inds_count = 0;
  var inds_pos = 0;
  while inds_count<ia.dom.num_inds {
    var ind = ia.dom.inds(inds_pos);
    if (ind.valid) {
      fwrite( f, ia.data(inds_pos));
      inds_count += 1;
      if (inds_count < ia.dom.num_inds) then fwrite( f, " ");
    }
    inds_pos += 1;
  }
}


// Thomas Wang's 64b mix function from http://www.concentric.net/~Ttwang/tech/inthash.htm
def _gen_key(i: int(64)): int(64) {
  var key = i;
  key += ~(key << 32);
  key = key ^ (key >> 22);
  key += ~(key << 13);
  key = key ^ (key >> 8);
  key += (key << 3);
  key = key ^ (key >> 15);
  key += ~(key << 27);
  key = key ^ (key >> 31);
  return key & 0x7fffffffffffffff;  // YAH, make non-negative
}

pragma "inline"
def _indefinite_hash(b: bool): int(64) {
  if (b) 
    return 17;
  else
    return 31;
}

pragma "inline"
def _indefinite_hash(i: int(64)): int(64) {
  return _gen_key(i);
}

pragma "inline"
def _indefinite_hash(u: uint(64)): int(64) {
  return _gen_key(u:int(64));
}

pragma "inline"
def _indefinite_hash(f: float): int(64) {
  return _gen_key(__primitive( "float2int", f));
}

pragma "inline"
def _indefinite_hash(c: complex): int(64) {
  return _gen_key(c.real:int ^ c.imag:int); 
}

// Use djb2 (Dan Bernstein in comp.lang.c.
pragma "inline"
def _indefinite_hash(x : string): int(64) {
  var hash: int(64) = 0;
  for c in 1..length(x) {
    hash = ((hash << 5) + hash) ^ ascii(x.substring(c));
  }
  return _gen_key(hash);
}

pragma "inline"
def _indefinite_hash(o: object): int(64) {
  return _gen_key(__primitive( "object2int", o));
}
