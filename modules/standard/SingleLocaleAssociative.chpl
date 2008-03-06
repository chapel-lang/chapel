use List;

param _MIN_SIZE = 0;
param _MAX_SIZE = 26;

param _EMPTY   = -1;
param _DELETED = -2;
param _NOPLACE = -3;



record _ind_data_t {
  type itype;
  var  data: itype;
  var  valid: bool;
}


class SingleLocaleAssociativeDomain: BaseDomain {
  param rank: int;
  type idxType;
  var _arrs2: list(BaseArray);    // WAW: unfortunately redundant list

  const _ps : [0..26] int = (23, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
                             24593, 49157, 98317, 196613, 393241, 786433, 
                             1572869, 3145739, 6291469, 12582917, 25165843, 
                             50331653, 100663319, 201326611, 402653189, 
                             805306457, 1610612741);


  var num_inds: int;
  var size: int = 0;
  var tableDom = [0.._ps(0)-1];
  var table: [tableDom] int = _EMPTY;
  var indsDom = [0.._ps(0)/2];
  var inds: [indsDom] _ind_data_t(idxType);
  var free_inds: _stack(int) = new _stack(int);





  def getIndices()
    return this; // stopgap measure given old implementation

  def setIndices(b: SingleLocaleAssociativeDomain) {
    oldAssignHelper(this, b);
  }

  def buildEmptyDomain()
    return new SingleLocaleAssociativeDomain(rank=rank,idxType=idxType);

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
    param debug = false;
    if (size > _MIN_SIZE) {
      var new_len = _ps(size-1);

      var new_tableDom = [0..new_len-1];
      var new_table: [new_tableDom] int = _EMPTY;

      var new_indsDom = [0..new_len/2-1];
      var new_inds: [new_indsDom] _ind_data_t(idxType);
      var old_map: [0..num_inds-1] int;
      var inds_count = 0;
      var inds_pos = 0;
      while inds_count<num_inds {
        var ind = inds(inds_pos);
        if (ind.valid) {
          old_map(inds_count) = table( _map(ind.data));
          new_inds(inds_count) = ind;
          new_table( _map(ind.data, false, new_table, new_inds)) = inds_count;
          inds_count += 1;
        }
        inds_pos += 1;
      }

      for ia in _arrs2 {
        ia._resize( new_len/2, old_map);
      }

      if debug then writeln("Assigning tableDom");
      tableDom = new_tableDom;
      if debug then writeln("Done");
      table = new_table;
      if debug then writeln("Assigning indsDom");
      indsDom = new_indsDom;
      if debug then writeln("Done");
      inds = new_inds;
      size -= 1;
      free_inds.empty();
    }
  }

  def _double() {
    param debug = false;
    if (size < _MAX_SIZE) {
      var new_len = _ps(size+1);

      var new_tableDom = [0..new_len-1];
      var new_table: [new_tableDom] int = _EMPTY;
      var new_indsDom = [0..new_len/2-1];
      var new_inds: [new_indsDom] _ind_data_t(idxType);
      var old_map: [inds.domain] int; // note: avoiding using indsDom here
                                      // because it causes all old_map arrays
                                      // to be reallocated whenever indsDom is
                                      // assigned ... :P  (BLC)
      for inds_pos in indsDom {
        var ind = inds(inds_pos);
        if (ind.valid) {                 // should be dense
          old_map(inds_pos) = table( _map(ind.data));
          new_inds(inds_pos) = ind;
          new_table( _map( ind.data, false, new_table, new_inds)) = inds_pos;
        } else {
          halt( "doubling without being full");
        }
      }

      for ia in _arrs2 {
        ia._resize( new_len/2, old_map);
      }

      if debug then writeln("Assigning tableDom (double)");
      tableDom = new_tableDom;
      if debug then writeln("Done");
      table = new_table;
      if debug then writeln("Assigning indsDom (double)");
      indsDom = new_indsDom;
      if debug then writeln("Done");
      inds = new_inds;
      size += 1;
      free_inds.empty();
    } else {
      halt( "cannot double associative domain");
    }
  }

  def _map(ind: idxType, deletedOK: bool = false,
           itable, iinds): int { // was: itable = table, iinds = inds; see below
    param debug = false;
    if debug then writeln("itable.domain = ", itable.domain);
    const base = _associative_hash(ind);
    for probe in itable.domain {
      var i = ((base + probe**2) % itable.numElements):int(32);
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

  // This version of _map is redundant with the previous, but
  // put here in order to avoid the array copies that are currently
  // inserted when default arguments of type array are used.
  def _map(ind: idxType, deletedOK: bool = false): int {
    param debug = false;
    if debug then writeln("table.domain = ", table.domain);
    const base = _associative_hash(ind);
    for probe in table.domain {
      var i = ((base + probe**2) % table.numElements):int(32);
      if debug then writeln("Trying i = ", i);
      var table_i = table(i);
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
          if debug then writeln("inds(table_i).data = ", inds(table_i).data);
          if inds(table_i).data==ind then
            return i;
        }
      }
    }
    return _NOPLACE;
  }

  def _get_index(ind : idxType) {
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
    var ia = new SingleLocaleAssociativeArray(eltType, idxType, dom=this); 
    _arrs2.append(ia);
    return ia;
  }

  // public routines

  def clear() {
    // BLC: implemented this for simplicity, not performance
    while (numIndices != 0) do
      removeOne();

    def removeOne() {
      // Since it's not safe to iterate over a hash table while you're
      // removing stuff from it, we only do one iteration at a time
      for i in these() {
        remove(i);
        return;
      }
    }
  }

  def add(ind : idxType) {
    var hash = _map(ind, deletedOK = true);
    var ind_pos = table(hash);
    if (ind_pos==_EMPTY || ind_pos==_DELETED) {

      if (free_inds.length > 0) {      // recycle
        ind_pos = free_inds.pop();            
      } else {
        if (num_inds == inds.numElements) {
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

  def remove( ind: idxType) {
    var ind_pos = _map(ind);
    if (ind_pos >= 0) {
      var table_i = table(ind_pos);
      if (table_i >= 0) {
        inds(table_i).valid = false;
        num_inds -= 1;
        free_inds.push( table_i);
        table(ind_pos) = _DELETED;
        
        for ia in _arrs2 do

          ia._purge( table_i);
      } else {
        halt("index not in domain: ", ind);
      }
    }

    if (size > _MIN_SIZE) then
      if (num_inds < _ps(size-1)/2) {  // downsize?
        _half();
      }
  }


  def member( ind: idxType) {
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
  var indices: list(a.idxType);
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
  type idxType;
  var dom : SingleLocaleAssociativeDomain(rank=1,idxType=idxType);

  const _ps : [0..26] int = (23, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
                             24593, 49157, 98317, 196613, 393241, 786433, 
                             1572869, 3145739, 6291469, 12582917, 25165843, 
                             50331653, 100663319, 201326611, 402653189, 
                             805306457, 1610612741);

  var dataDom = [0.._ps(0)/2];
  var data : [dataDom] eltType;

  // This method is unsatisfactory -- see bradc's commit entries of
  // 01/02/08 around 14:30 for details
  def _purge( ind: int) {
    var d: eltType;
    data( ind) = d;
  }

  def _resize( length: int, old_map) {
    var new_dataDom = [0..length-1];
    var new_data: [new_dataDom] eltType;
    for i in old_map.domain {
      new_data(i) = data(old_map(i));
    }
    dataDom = new_dataDom;
    data = new_data;
  }

  def this(ind : idxType) var : eltType
    return data(dom._get_index(ind));

  def these() var {
    var i = 0;
    var numFound = 0;
    while numFound < dom.num_inds {
      if (dom.inds(i).valid) {
        numFound += 1;
        yield data(i);
      }
      i += 1;
    }
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
      while (inds_pos < dom.num_inds) && (inds_count < dom.inds.numElements) {
	var ind = dom.inds(inds_count);
	if ind.valid {
	  var i = td._map( ind.data);
	  if (i < 0) then {
	    _purge( dom.table(dom._map( ind.data)));
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

  def tupleInit(b: _tuple) {
    for param i in 1..b.size do
      data(i-1) = b(i);
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
def _associative_hash(b: bool): int(64) {
  if (b) 
    return 17;
  else
    return 31;
}

pragma "inline"
def _associative_hash(i: int(64)): int(64) {
  return _gen_key(i);
}

pragma "inline"
def _associative_hash(u: uint(64)): int(64) {
  return _gen_key(u:int(64));
}

pragma "inline"
def _associative_hash(f: real): int(64) {
  return _gen_key(__primitive( "real2int", f));
}

pragma "inline"
def _associative_hash(c: complex): int(64) {
  return _gen_key(c.re:int ^ c.im:int); 
}

// Use djb2 (Dan Bernstein in comp.lang.c.
pragma "inline"
def _associative_hash(x : string): int(64) {
  var hash: int(64) = 0;
  for c in 1..length(x) {
    hash = ((hash << 5) + hash) ^ ascii(x.substring(c));
  }
  return _gen_key(hash);
}

pragma "inline"
def _associative_hash(o: object): int(64) {
  return _gen_key(__primitive( "object2int", o));
}
