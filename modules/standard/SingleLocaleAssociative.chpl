use List;

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
param _NOPLACE = -3;



record _ind_data_t {
  type itype;
  var  data: itype;
  var  valid: bool;
}


class SingleLocaleAssociativeDomain: BaseDomain {
  type ind_type;
  var _arrs2: list(BaseArray);    // WAW: unfortunately redundant list
  var num_inds: int;
  var size: int = 0;
  var table: _ddata(int);
  var inds: _ddata(_ind_data_t(ind_type));
  var free_inds: _stack(int);

  def getIndices()
    return this; // stopgap measure given old implementation

  def setIndices(b: SingleLocaleAssociativeDomain) {
    this = oldAssignHelper(this, b);
  }

  def buildEmptyDomain()
    return SingleLocaleAssociativeDomain(ind_type=ind_type);

  // compiler-internal routines

  def initialize() {
    table  = _ddata(int, _ps(size));
    table.init( _EMPTY);
    inds =  _ddata(_ind_data_t(ind_type), _ps(size)/2);
    inds.init();
    free_inds = _stack(int);
  }

  def these() {
    var i = 0;
    var numFound = 0;
    while numFound < num_inds {
      if (inds(i).valid) {
        numFound += 1;
        yield inds(i).data;
      }
      i += 1;
    }
  }

  // internal routines

  def _half() {
    if (size > _MIN_SIZE) {
      var new_len = _ps(size-1);

      var new_table = _ddata(int, new_len);
      new_table.init(_EMPTY);

      var new_inds = _ddata(_ind_data_t(ind_type), new_len/2);
      new_inds.init();
      var old_map: _ddata(int);
      old_map = _ddata( int, num_inds);
      old_map.init();
      var inds_count = 0;
      var inds_pos = 0;
      while inds_count<num_inds {
        var ind = inds(inds_pos);
        if (ind.valid) {
          old_map(inds_count) = table( _map(ind.data));
          new_inds(inds_count) = ind;
          new_table( _map(ind.data, new_table, new_inds)) = inds_count;
          inds_count += 1;
        }
        inds_pos += 1;
      }

      for ia in _arrs2 {
        ia._resize( new_len/2, old_map);
      }

      table = new_table;
      inds = new_inds;
      size -= 1;
      free_inds.empty();
    }
  }

  def _double() {
    if (size < _MAX_SIZE) {
      var new_len = _ps(size+1);

      var new_table = _ddata( int, new_len);
      new_table.init( _EMPTY);
      var new_inds = _ddata( _ind_data_t(ind_type), new_len/2);
      new_inds.init();
      var old_map: _ddata(int);
      old_map = _ddata( int, inds.size);
      old_map.init();
      for inds_pos in 0..inds.size-1 {  
        var ind = inds(inds_pos);
        if (ind.valid) {                 // should be dense
          old_map(inds_pos) = table( _map(ind.data));
          new_inds(inds_pos) = ind;
          new_table( _map( ind.data, new_table, new_inds)) = inds_pos;
        } else {
          halt( "doubling without being full");
        }
      }

      for ia in _arrs2 {
        ia._resize( new_len/2, old_map);
      }

      table = new_table;
      inds = new_inds;
      size += 1;
      free_inds.empty();
    } else {
      halt( "cannot double indefinite domain");
    }
  }

  def _map(ind: ind_type, 
           itable: _ddata(int) = table, 
           iinds: _ddata(_ind_data_t(ind_type)) = inds,
           deletedOK: bool = false): int {
    param debug = false;
    if debug then writeln("itable.size = ", itable.size);
    const base = _indefinite_hash(ind);
    for probe in 0..itable.size {
      var i = ((base + probe**2) % itable.size):int(32);
      if debug then writeln("Trying i = ", i);
      var table_i = itable(i);
      if debug then writeln("table_i = ", table_i);
      select (table_i) {
        when _EMPTY {
          if debug then writeln("EMPTY");
          return i;
        }
        when _DELETED {
          if debug then writeln("DELETED");
          if deletedOK then
            return i;
          if debug then writeln("...but not OK");
        }
        otherwise {
          if debug then writeln("iinds(table_i).data = ", iinds(table_i).data);
          if iinds(table_i).data==ind then
            return i;
        }
      }
    }
    return _NOPLACE;
  }

  def _get_index(ind : ind_type) {
    var i = _map(ind);
    if (table(i) < 0) then
      halt("array index out of bounds: ", ind);
    if (i >= 0) then
      return table(i);
    else
      halt( "index not found: ", ind);
    // will never get here, but to avoid compiler warnings:
    return 0;
  }

  def buildArray(type eltType) {
    var ia = SingleLocaleAssociativeArray(eltType, ind_type, dom=this); 
    _arrs2.append(ia);
    return ia;
  }

  // public routines

  def add(ind : ind_type) {
    var hash = _map(ind, deletedOK = true);
    var ind_pos = table(hash);
    if (ind_pos==_EMPTY || ind_pos==_DELETED) {

      if (free_inds.length > 0) {      // recycle
        ind_pos = free_inds.pop();            
      } else {
        if (num_inds == inds.size) {
          _double();
        }
        ind_pos = num_inds;
        hash = _map(ind, deletedOK = true);
      }
      table(hash) = ind_pos;
      inds(ind_pos).data = ind;
      inds(ind_pos).valid = true;
      num_inds += 1;
    } // else, already in domain
  }

  def remove( ind: ind_type) {
    var ind_pos = _map(ind);
    if (ind_pos >= 0) {
      var table_i = table(ind_pos);
      if (table_i >= 0) {
        inds(table_i).valid = false;
        num_inds -= 1;
        free_inds.push( table_i);
        table(ind_pos) = _DELETED;
        
        for ia in _arrs2 do
          ia.purge( table_i);
      } else {
        halt("index not in domain: ", ind);
      }
    }

    if (size > _MIN_SIZE) then
      if (num_inds < _ps(size-1)/2) {  // downsize?
        _half();
      }
  }


  def member( ind: ind_type) {
    const mapind = _map(ind);
    if (mapind >= 0) {
      var ind_pos = table(mapind);
      return (ind_pos >= 0);
    }
    return false;
  }

  def numIndices {
    return num_inds;
  }
}


def oldAssignHelper(a: SingleLocaleAssociativeDomain, b: SingleLocaleAssociativeDomain) {
  var indices: list(a.ind_type);
  var inds_count = 0;
  var inds_pos = 0;
  var total_inds = a.num_inds;
  while inds_count<total_inds {
    var ind = a.inds( inds_pos);
    if (ind.valid) {
      indices.append(ind.data);
      inds_count += 1;
    }
    inds_pos += 1;
  }

  for ind in indices {
    if (!b.member(ind)) {
      a.remove( ind);
    }
  }

  inds_count = 0;
  inds_pos = 0;
  total_inds = b.num_inds;
  while inds_count<total_inds {
    var ind = b.inds( inds_pos);
    if (ind.valid) {
      if (!a.member(ind.data)) {
        a.add( ind.data);
      }
      inds_count += 1;
    }
    inds_pos += 1;
  }
  return a;  // WAW: YAH, "a" instead of "b" because we want to match keys, not do a deep copy (e.g., _arrs2)
}


def SingleLocaleAssociativeDomain.writeThis(f: Writer) {
  f.write("[");
  var inds_count = 0;
  var inds_pos = 0;
  while inds_count<num_inds {
    var ind = inds( inds_pos);
    if (ind.valid) {
      f.write(ind.data);
      inds_count += 1;
      if (inds_count<num_inds) then f.write(", ");
    }
    inds_pos += 1;
  }
  f.write("]");
}


class SingleLocaleAssociativeArray: BaseArray {
  type eltType;
  type ind_type;
  var dom : SingleLocaleAssociativeDomain(ind_type=ind_type);
  var data : _ddata(eltType);

  def initialize() {
    data = _ddata( eltType, _ps(0)/2);
    data.init();
  }

  def purge( ind: int) {
    var d: eltType;
    data( ind) = d;
  }

  def _resize( length: int, old_map: _ddata(int)) {
    var new_data: _ddata(eltType) = _ddata( eltType, length);
    new_data.init();
    for i in 0..old_map.size-1 {
      new_data(i) = data(old_map(i));
    }
    data = new_data;
  }

  def this(ind : ind_type) var : eltType
    return data(dom._get_index(ind));

  def these() var {
    for i in 0..dom.num_inds-1 do
      yield data(i);
  }

  // WAW: hack due to domain-array interface. The array is not reallocated
  // here. Values of indices not in the new domain are reset. The 
  // domain assignment will perform the reallocation of each array that
  // maps to it.
  def reallocate(d: _domain) {
    if (d._value.type == dom.type) {
      var td = d._value;
      var inds_count = 0;
      var inds_pos = 0;
      while (inds_pos < dom.num_inds) && (inds_count < dom.inds.size) {
	var ind = dom.inds(inds_count);
	if ind.valid {
	  var i = td._map( ind.data);
	  if (i < 0) then {
	    purge( dom.table(dom._map( ind.data)));
	  }
	  inds_pos += 1;
	}
	inds_count += 1;
      }
    }
  }

  def numElements {
    return dom.numIndices;
  }
}

def SingleLocaleAssociativeArray.writeThis(f: Writer) {
  var inds_count = 0;
  var inds_pos = 0;
  while inds_count<dom.num_inds {
    var ind = dom.inds(inds_pos);
    if (ind.valid) {
      f.write(data(inds_pos));
      inds_count += 1;
      if (inds_count < dom.num_inds) then f.write(" ");
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
def _indefinite_hash(f: real): int(64) {
  return _gen_key(__primitive( "real2int", f));
}

pragma "inline"
def _indefinite_hash(c: complex): int(64) {
  return _gen_key(c.re:int ^ c.im:int); 
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
