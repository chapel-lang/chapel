use Sort /* only QuickSort */;

// TODO: make the domain parameterized by this?
// TODO: make int(64) the default index type here and in arithemtic domains
type chpl_table_index_type = int(32);


/* These declarations could/should both be nested within
   DefaultAssociativeDomain? */
enum chpl_hash_status { empty, full, deleted };

record chpl_TableEntry {
  type idxType;
  var status: chpl_hash_status = chpl_hash_status.empty;
  var idx: idxType;
}

def chpl__primes return (23, 53, 97, 193, 389, 769, 1543,
                         3079, 6151, 12289, 24593, 49157, 98317, 196613,
                         393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
                         50331653, 100663319, 201326611, 402653189, 805306457, 1610612741);

class DefaultAssociativeDomain: BaseAssociativeDomain {
  type idxType;

  var dist: DefaultDistribution;

  // The guts of the associative domain
  var numEntries: chpl_table_index_type = 0;
  var tableSizeNum = 1;
  var tableSize = chpl__primes(tableSizeNum);
  var tableDom = [0..tableSize-1];
  var table: [tableDom] chpl_TableEntry(idxType);


  // Temporary arrays for side computations:
  // TODO: Slosh back and forth between two tables rather than copying back
  // TODO: This ugly [0..-1] domain appears several times in the code --
  //       replace with a named constant/param?
  var tmpDom = [0..-1:chpl_table_index_type];
  var tmpTable: [tmpDom] chpl_TableEntry(idxType);
  var tmpDom2 = [0..-1:chpl_table_index_type];
  var tmpTable2: [tmpDom2] idxType;

  def DefaultAssociativeDomain(type idxType, dist: DefaultDistribution) {
    this.dist = dist;
  }

  //
  // Standard Internal Domain Interface
  //
  def buildArray(type eltType) {
    return new DefaultAssociativeArray(eltType, idxType, dom=this); 
  }

  def getIndices()
    return this;

  def setIndices(b: DefaultAssociativeDomain) {
    // store cache of old domain/arrays
    _backupArrays();
    tmpDom = tableDom;
    tmpTable = table;

    // clear out old table
    numEntries = 0;
    tableDom = [0..-1:chpl_table_index_type];

    // choose new table size
    for primeNum in 1..chpl__primes.size {
      if (b.numEntries * 2 < chpl__primes(primeNum)) {
        tableSizeNum = primeNum;
        break;
      }
    }
    tableSize = chpl__primes(tableSizeNum);
    tableDom = [0..tableSize-1];

    // add indices and copy array data over
    for idx in b {
      const newSlotNum = add(idx);
      const (foundit, slot) = _findFilledSlot(idx, tmpTable);
      if (foundit) {
        _preserveArrayElement(oldslot=slot, newslot=newSlotNum);
      }
    }
    tmpDom = [0..-1:chpl_table_index_type];
    tmpDom2 = [0..-1:chpl_table_index_type];
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
      table(slot).status = chpl_hash_status.empty;
    }
    numEntries = 0;
  }

  def member(idx: idxType): bool {
    return _findFilledSlot(idx)(1);
  }

  def add(idx: idxType): index(tableDom) {
    if ((numEntries+1)*2 > tableSize) {
      // TODO: should also make sure that we don't exceed the maximum table
      // size
      _resize(grow=true);
    }
    const (foundSlot, slotNum) = _findEmptySlot(idx);
    if (foundSlot) {
      table(slotNum).status = chpl_hash_status.full;
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
    const (foundSlot, slotNum) = _findFilledSlot(idx);
    if (foundSlot) {
      table(slotNum).status = chpl_hash_status.deleted;
      numEntries -= 1;
    } else {
      halt("index not in domain: ", idx);
    }
    if (numEntries*8 < tableSize && tableSizeNum > 1) {
      _resize(grow=false);
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
    var count: chpl_table_index_type = 0;
    for slot in _fullSlots() {
      tmpTable2(count) = table(slot).idx;
      count += 1;
    }
    QuickSort(tmpTable2);
  }

  def _destroySortedTmpTable2() {
    tmpDom2 = [0..-1:chpl_table_index_type];
  }

  def _resize(grow:bool) {
    // back up the arrays
    _backupArrays();

    // copy the table (TODO: could use swap between two versions)
    tmpDom = tableDom;
    tmpTable = table;

    // grow original table
    tableDom = [0..-1:chpl_table_index_type]; // non-preserving resize
    numEntries = 0; // reset, because the adds below will re-set this
    tableSizeNum += if grow then 1 else -1;
    tableSize = chpl__primes(tableSizeNum);
    tableDom = [0..tableSize-1];

    // insert old data into newly resized table
    for slot in _fullSlots(tmpTable) {
      const newslot = add(tmpTable(slot).idx);
      _preserveArrayElement(oldslot=slot, newslot=newslot);
    }
    
    // deallocate tmpTable
    tmpDom = [0..-1:chpl_table_index_type];
    _removeArrayBackups();
  }

  def _findFilledSlot(idx: idxType, tab = table): (bool, index(tableDom)) {
    for slotNum in _lookForSlots(idx, tab.domain.high+1) {
      const slotStatus = tab(slotNum).status;
      if (slotStatus == chpl_hash_status.empty) {
        return (false, -1);
      } else if (slotStatus == chpl_hash_status.full) {
        if (tab(slotNum).idx == idx) {
          return (true, slotNum);
        }
      }
    }
    return (false, -1);
  }

  def _findEmptySlot(idx: idxType): (bool, index(tableDom)) {
    for slotNum in _lookForSlots(idx) {
      const slotStatus = table(slotNum).status;
      if (slotStatus == chpl_hash_status.empty ||
          slotStatus == chpl_hash_status.deleted) {
        return (true, slotNum);
      } else if (table(slotNum).idx == idx) {
        return (false, slotNum);
      }
    }
    return (false, -1);
  }
    
  def _lookForSlots(idx: idxType, numSlots = tableSize) {
    const baseSlot = _associative_hash_wrapper(idx);
    for probe in 0..numSlots/2 {
      yield (baseSlot + probe**2)%numSlots;
    }
  }

  def _fullSlots(tab = table) {
    for slot in tab.domain {
      if tab(slot).status == chpl_hash_status.full then
        yield slot;
    }
  }
}


class DefaultAssociativeArray: BaseArray {
  type eltType;
  type idxType;
  var dom : DefaultAssociativeDomain(idxType);

  var data : [dom.tableDom] eltType;

  var tmpDom = [0..-1:chpl_table_index_type];
  var tmpTable: [tmpDom] eltType;

  //
  // Standard internal array interface
  // 

  def reallocate(d: domain) {
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
    tmpDom = [0..-1:chpl_table_index_type];
  }

  def _preserveArrayElement(oldslot, newslot) {
    data(newslot) = tmpTable(oldslot);
  }

  // private internal interface

  def _createSortedTmpTable() {
    tmpDom = [0..numElements-1];
    var count: chpl_table_index_type = 0;
    for slot in dom._fullSlots() {
      tmpTable(count) = data(slot);
      count += 1;
    }
    QuickSort(tmpTable);
  }

  def _destroySortedTmpTable() {
    tmpDom = [0..-1:chpl_table_index_type];
  }
}


def _associative_hash_wrapper(x): chpl_table_index_type {
  const hash = _associative_hash(x); 
  return (hash & max(chpl_table_index_type)): chpl_table_index_type;
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
  if (b) then
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
