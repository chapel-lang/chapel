//
// Abstract distribution class
//
class BaseDist {
  def newArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    compilerError("arithmetic domains not supported by this distribution");
  }

  def newAssociativeDom(type idxType) {
    compilerError("associative domains not supported by this distribution");
  }

  def newEnumDom(type idxType) {
    compilerError("enumerated domains not supported by this distribution");
  }

  def newOpaqueDom(type idxType) {
    compilerError("opaque domains not supported by this distribution");
  }

  def newSparseDom(param rank: int, type idxType, dom: domain) {
    compilerError("opaque domains not supported by this distribution");
  }
}

//
// Abstract domain classes
//
class BaseDom {
  var _domCnt$: sync int = 1; // domain reference count and lock
  var _arrs: list(BaseArr);   // arrays declared over this domain


  def destroyDom(arr: BaseArr = nil) {
    var cnt = _domCnt$ - 1;
    if cnt < 0 then
      halt("domain reference count is negative!");
    if arr then
      on arr do
        _arrs.remove(arr);
    _domCnt$ = cnt;
    return cnt;
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

  def supportsPrivatization() param return false;
}

class BaseArithmeticDom : BaseDom {
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

class BaseSparseDom : BaseDom {
  def clear() {
    halt("clear not implemented for this distribution");
  }

  def clearForIteratableAssign() {
    clear();
  }
}

class BaseAssociativeDom : BaseDom {
  def clear() {
    halt("clear not implemented for this distribution");
  }

  def clearForIteratableAssign() {
    clear();
  }
}

class BaseOpaqueDom : BaseDom {
  def clear() {
    halt("clear not implemented for this distribution");
  }

  def clearForIteratableAssign() {
    clear();
  }
}

class BaseEnumDom : BaseDom {
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
class BaseArr {
  var _arrCnt$: sync int = 1; // array reference count (and eventually lock)
  var _arrAlias: BaseArr;     // reference to base array if an alias

  def canCopyFromDevice param return false;
  def canCopyFromHost param return false;

  def getBaseDom(): BaseDom {
    return nil;
  }

  def destroyArr(): int {
    var cnt = _arrCnt$ - 1;
    if cnt < 0 then
      halt("array reference count is negative!");
    _arrCnt$ = cnt;
    if cnt == 0 {
      var dom = getBaseDom();
      on dom {
        var cnt = dom.destroyDom(this);
        if cnt == 0 then
          delete dom;
      }
      if _arrAlias {
        on _arrAlias {
          var cnt = _arrAlias.destroyArr();
          if cnt == 0 then
            delete _arrAlias;
        }
      } else {
        destroyData();
      }
    }
    return cnt;
  }

  def destroyData() { }

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

  def supportsPrivatization() param return false;
}
