use Sort /* only QuickSort */;

// TODO: make the domain parameterized by this?
// TODO: make int(64) the default index type here and in arithemtic domains
type _chpl_table_index_type = int(32);


/* These declarations could/should both be nested within
   SingleLocaleAssociativeDomain? */
enum _chpl_hash_status { empty, full, deleted };

record _chpl_TableEntry {
  type idxType;
  var status: _chpl_hash_status = _chpl_hash_status.empty;
  var idx: idxType;
}


class SingleLocaleAssociativeDomain: BaseDomain {
  type idxType;

  // TODO: move this out of here once the initializers can refer to global
  // variables (see David's future of 03/07/08)
  const _primes : [0..26] _chpl_table_index_type 
                = (23, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 
                   49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 
                   6291469, 12582917, 25165843, 50331653, 100663319, 201326611,
                   402653189, 805306457, 1610612741);



  // The guts of the associative domain
  var numEntries: _chpl_table_index_type = 0;
  var tableSizeNum = 0;
  var tableSize = _primes(tableSizeNum);
  var tableDom = [0..tableSize-1];
  var table: [tableDom] _chpl_TableEntry(idxType);


  // Temporary arrays for side computations:
  // TODO: Slosh back and forth between two tables rather than copying back
  // TODO: This ugly [0..-1] domain appears several times in the code --
  //       replace with a named constant/param?
  var tmpDom = [0..-1:_chpl_table_index_type];
  var tmpTable: [tmpDom] _chpl_TableEntry(idxType);
  var tmpDom2 = [0..-1:_chpl_table_index_type];
  var tmpTable2: [tmpDom2] idxType;

  // BLC: Didn't want to have this, but _wrapDomain requires it
  param rank = 1;
  

  //
  // Standard Internal Domain Interface
  //
  def buildEmptyDomain() {
    return new SingleLocaleAssociativeDomain(idxType=idxType);
  }

  def buildArray(type eltType) {
    if (this == nil) then
      return new SingleLocaleAssociativeArray(eltType, idxType, dom=new SingleLocaleAssociativeDomain(idxType=idxType)); 
    else 
      return new SingleLocaleAssociativeArray(eltType, idxType, dom=this); 
  }

  def getIndices()
    return this;

  def setIndices(b: SingleLocaleAssociativeDomain) {
    // store cache of old domain/arrays
    _backupArrays();
    tmpDom = tableDom;
    tmpTable = table;

    // clear out old table
    numEntries = 0;
    tableDom = [0..-1:_chpl_table_index_type];

    // choose new table size
    for primeNum in _primes.domain {
      if (b.numEntries * 2 < _primes(primeNum)) {
        tableSizeNum = primeNum;
        break;
      }
    }
    tableSize = _primes(tableSizeNum);
    tableDom = [0..tableSize-1];

    // add indices and copy array data over
    for idx in b {
      const newSlotNum = add(idx);
      const (foundit, slot) = _findFilledSlotInTmp(idx);
      if (foundit) {
        _preserveArrayElement(oldslot=slot, newslot=newSlotNum);
      }
    }
    tmpDom = [0..-1:_chpl_table_index_type];
    tmpDom2 = [0..-1:_chpl_table_index_type];
    _removeArrayBackups();
  }
  
  def writeThis(f: Writer) {
    var first = true;
    f.write("[");
    for slot in _fullSlots() {
      if first then 
        first = false; 
      else 
        f.write(", ");
      f.write(table(slot).idx);
    }
    f.write("]");
  }

  //
  // Standard user domain interface
  //

  def numIndices {
    return numEntries;
  }

  def these() {
    for slot in _fullSlots() {
      yield table(slot).idx;
    }
  }

  //
  // Associative Domain Interface
  //
  def clear() {
    for slot in tableDom {
      table(slot).status = _chpl_hash_status.empty;
    }
    numEntries = 0;
  }

  def member(idx: idxType): bool {
    return _findFilledSlot(idx)(1);
  }

  def add(idx: idxType): index(tableDom) {
    if ((numEntries+1)*2 > tableSize) {
      _grow();
    }
    const (foundSlot, slotNum) = _findEmptySlot(idx);
    if (foundSlot) {
      table(slotNum).status = _chpl_hash_status.full;
      table(slotNum).idx = idx;
      numEntries += 1;
    } else {
      if (slotNum < 0) {
        halt("couldn't add ", idx, " -- ", numEntries, " / ", tableSize, " taken");
        return -1;
      }
      // otherwise, re-adding an index that's already in there
    }
    return slotNum;
  }

  def remove(idx: idxType) {
    // TODO: shrink table if less than 1/4 full
    const (foundSlot, slotNum) = _findFilledSlot(idx);
    if (foundSlot) {
      table(slotNum).status = _chpl_hash_status.deleted;
      numEntries -= 1;
    } else {
      halt("index not in domain: ", idx);
    }
  }

  def sorted() {
    _createSortedTmpTable2();
    for ind in tmpTable2 do
      yield ind;
    _destroySortedTmpTable2();
  }

  //
  // Internal interface (private)
  //
  def _createSortedTmpTable2() {
    // TODO: should assert that tmpDom2 is non-empty to avoid conflicting
    // calls? Or not -- to support early exit from iterator?
    tmpDom2 = [0..numEntries-1];
    var count: _chpl_table_index_type = 0;
    for slot in _fullSlots() {
      tmpTable2(count) = table(slot).idx;
      count += 1;
    }
    QuickSort(tmpTable2);
  }

  def _destroySortedTmpTable2() {
    tmpDom2 = [0..-1:_chpl_table_index_type];
  }

  def _grow() {
    // back up the arrays
    _backupArrays();

    // copy the table (TODO: could use swap between two versions)
    tmpDom = tableDom;
    tmpTable = table;

    // grow original table
    tableDom = [0..-1:_chpl_table_index_type]; // non-preserving resize
    numEntries = 0; // reset, because the adds below will re-set this
    tableSizeNum += 1;
    tableSize = _primes(tableSizeNum);
    tableDom = [0..tableSize-1];

    // insert old data into newly resized table
    for slot in _fullSlotsInTmp() {
      const newslot = add(tmpTable(slot).idx);
      _preserveArrayElement(oldslot=slot, newslot=newslot);
    }
    
    // deallocate tmpTable
    tmpDom = [0..-1:_chpl_table_index_type];
    _removeArrayBackups();
  }

  def _findFilledSlot(idx: idxType): (bool, index(tableDom)) {
    for slotNum in _lookForSlots(idx) {
      const slotStatus = table(slotNum).status;
      if (slotStatus == _chpl_hash_status.empty) {
        return (false, -1);
      } else if (slotStatus == _chpl_hash_status.full) {
        if (table(slotNum).idx == idx) {
          return (true, slotNum);
        }
      }
    }
    return (false, -1);
  }

  // This is redundant with the above, but implemented this way to avoid
  // using default array arguments which are currently resulting in copies
  def _findFilledSlotInTmp(idx: idxType): (bool, index(tableDom)) {
    for slotNum in _lookForSlotsInTmp(idx) {
      const slotStatus = tmpTable(slotNum).status;
      if (slotStatus == _chpl_hash_status.empty) {
        return (false, -1);
      } else if (slotStatus == _chpl_hash_status.full) {
        if (tmpTable(slotNum).idx == idx) {
          return (true, slotNum);
        }
      }
    }
    return (false, -1);
  }

  def _findEmptySlot(idx: idxType): (bool, index(tableDom)) {
    for slotNum in _lookForSlots(idx) {
      const slotStatus = table(slotNum).status;
      if (slotStatus == _chpl_hash_status.empty ||
          slotStatus == _chpl_hash_status.deleted) {
        return (true, slotNum);
      } else if (table(slotNum).idx == idx) {
        return (false, slotNum);
      }
    }
    return (false, -1);
  }
    
  def _lookForSlots(idx: idxType) {
    const baseSlot = _associative_hash_wrapper(idx);
    for probe in 0..tableSize/2 {
      const tmp = baseSlot + probe**2;
      const tmp2 = (baseSlot + probe**2)&tableSize;
      yield (baseSlot + probe**2)%tableSize;
    }
  }

  // TODO: Remove this and the other inTmp thing
  def _lookForSlotsInTmp(idx: idxType) {
    const baseSlot = _associative_hash_wrapper(idx);
    for probe in 0..(tmpTable.domain.high+1)/2 {
      yield (baseSlot + probe**2)%(tmpTable.domain.high+1);
    }
  }

  def _fullSlots() {
    for slot in tableDom {
      if table(slot).status == _chpl_hash_status.full then
        yield slot;
    }
  }

  // TODO: Remove this and the other inTmp things
  def _fullSlotsInTmp() {
    for slot in tmpDom {
      if tmpTable(slot).status == _chpl_hash_status.full then
        yield slot;
    }
  }
}


class SingleLocaleAssociativeArray: BaseArray {
  type eltType;
  type idxType;
  var dom : SingleLocaleAssociativeDomain(idxType=idxType);

  var data : [dom.tableDom] eltType;

  var tmpDom = [0..-1:_chpl_table_index_type];
  var tmpTable: [tmpDom] eltType;

  //
  // Standard internal array interface
  // 

  def reallocate(d: _domain) {
    // reallocation is done in the setIndices function
  }


  //
  // Standard array interface
  //
  def numElements {
    return dom.numIndices;
  }

  def this(idx : idxType) var : eltType {
    const (found, slotNum) = dom._findFilledSlot(idx);
    if (found) then
      return data(slotNum);
    else {
      halt("array index out of bounds: ", idx);
      return data(0);
    }
  }

  def these() var {
    for slot in dom._fullSlots() {
      yield data(slot);
    }
  }

  def writeThis(f: Writer) {
    var first = true;
    for slot in dom._fullSlots() {
      if (first) then
        first = false;
      else
        f.write(" ");
      f.write(data(slot));
    }
  }


  //
  // Associative array interface
  //

  def sorted() {
    _createSortedTmpTable();
    for elem in tmpTable do
      yield elem;
    _destroySortedTmpTable();
  }


  //
  // Internal associative array interface
  //

  def _backupArray() {
    tmpDom = dom.tableDom;
    tmpTable = data;
  }

  def _removeArrayBackup() {
    tmpDom = [0..-1:_chpl_table_index_type];
  }

  def _preserveArrayElement(oldslot, newslot) {
    data(newslot) = tmpTable(oldslot);
  }

  // private internal interface

  def _createSortedTmpTable() {
    tmpDom = [0..numElements-1];
    var count: _chpl_table_index_type = 0;
    for slot in dom._fullSlots() {
      tmpTable(count) = data(slot);
      count += 1;
    }
    QuickSort(tmpTable);
  }

  def _destroySortedTmpTable() {
    tmpDom = [0..-1:_chpl_table_index_type];
  }
}


def _associative_hash_wrapper(x): _chpl_table_index_type {
  const hash = _associative_hash(x); 
  return (hash & max(_chpl_table_index_type)): _chpl_table_index_type;
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
  return (key & max(int(64))): int(64);  // YAH, make non-negative
}

pragma "inline"
def _associative_hash(b: bool): int(64) {
  if (b) 
    return 0;
  else
    return 1;
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
  return _gen_key(__primitive("real2int", c.re) ^ __primitive("real2int", c.im)); 
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
