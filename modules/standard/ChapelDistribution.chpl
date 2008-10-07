//
// Abstract distribution class
//
class Distribution {
  def newArithmeticDomain(param rank: int, type idxType, param stridable: bool) {
    compilerError("arithmetic domains not supported by this distribution");
  }

  def newAssociativeDomain(type idxType) {
    compilerError("associative domains not supported by this distribution");
  }

  def newEnumeratedDomain(type idxType) {
    compilerError("enumerated domains not supported by this distribution");
  }

  def newOpaqueDomain(type idxType) {
    compilerError("opaque domains not supported by this distribution");
  }

  def newSparseDomain(param rank: int, type idxType, dom: domain) {
    compilerError("opaque domains not supported by this distribution");
  }
}

//
// Abstract domain classes
//
class BaseDomain {
  var _arrs: list(BaseArray);
  var _count: sync int = 0;

  def ~BaseDomain() {
    _arrs.destroy();
    delete _count;
  }

  def member(ind) : bool {
    halt("membership test not supported for this domain type");
    return false;
  }

  // used for associative domains/arrays
  def _backupArrays() {
    for arr in _arrs do
      arr._backupArray();
  }

  def _removeArrayBackups() {
    for arr in _arrs do
      arr._removeArrayBackup();
  }

  def _preserveArrayElement(oldslot, newslot) {
    for arr in _arrs do
      arr._preserveArrayElement(oldslot, newslot);
  }
}

class BaseArithmeticDomain : BaseDomain {
  def clear() {
    halt("clear not implemented for this distribution");
  }

  def clearForIteratableAssign() {
    compilerError("Illegal assignment to an arithmetic domain");
  }

  def add(x) {
    compilerError("Cannot add indices to an arithmetic domain");
  }
}

class BaseSparseDomain : BaseDomain {
  def clear() {
    halt("clear not implemented for this distribution");
  }

  def clearForIteratableAssign() {
    clear();
  }
}

class BaseAssociativeDomain : BaseDomain {
  def clear() {
    halt("clear not implemented for this distribution");
  }

  def clearForIteratableAssign() {
    clear();
  }
}

class BaseOpaqueDomain : BaseDomain {
  def clear() {
    halt("clear not implemented for this distribution");
  }

  def clearForIteratableAssign() {
    clear();
  }
}

class BaseEnumDomain : BaseDomain {
  def clear() {
    compilerError("Cannot clear an enumerated domain");
  }

  def clearForIteratableAssign() {
    compilerError("Illegal assignment to an enumerated domain");
  }
}

//
// Abstract array class
//
pragma "base array"
class BaseArray {
  def reallocate(d: domain) {
    halt("reallocating not supported for this array type");
  }

  // This method is unsatisfactory -- see bradc's commit entries of
  // 01/02/08 around 14:30 for details
  def _purge( ind: int) {
    halt("purging not supported for this array type");
  }

  def _resize( length: int, old_map) {
    halt("resizing not supported for this array type");
  }

  def sparseShiftArray(shiftrange, initrange) {
    halt("sparseGrowDomain not supported for non-sparse arrays");
  }

  // methods for associative arrays
  def _backupArray() {
    halt("_backupArray() not supported for non-associative arrays");
  }

  def _removeArrayBackup() {
    halt("_removeArrayBackup() not supported for non-associative arrays");
  }

  def _preserveArrayElement(oldslot, newslot) {
    halt("_preserveArrayElement() not supported for non-associative arrays");
  }

  def supportsAlignedFollower() param return false;
}
