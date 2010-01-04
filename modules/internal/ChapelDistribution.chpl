use List;

config const minElemsPerChunk: uint(64) = 1;
config const maxChunks: int = -1;

//
// Abstract distribution class
//
class BaseDist {
  var _distCnt$: sync int = 0; // distribution reference count and lock
  var _doms: list(BaseDom);    // arrays declared over this domain

  pragma "dont disable remote value forwarding"
  def destroyDist(dom: BaseDom = nil) {
    var cnt = _distCnt$ - 1;
    if cnt < 0 then
      halt("distribution reference count is negative!");
    if dom then
      on dom do
        _doms.remove(dom);
    _distCnt$ = cnt;
    return cnt;
  }

  def newArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    compilerError("arithmetic domains not supported by this distribution");
  }

  def newAssociativeDom(type idxType) {
    compilerError("associative domains not supported by this distribution");
  }

  def newAssociativeDom(type idxType) where __primitive("isEnumType", idxType) {
    compilerError("enumerated domains not supported by this distribution");
  }

  def newOpaqueDom(type idxType) {
    compilerError("opaque domains not supported by this distribution");
  }

  def newSparseDom(param rank: int, type idxType, dom: domain) {
    compilerError("sparse domains not supported by this distribution");
  }

  def supportsPrivatization() param return false;
  def requiresPrivatization() param return false;

  def destroyDistributionDescriptor() { }
}

//
// Abstract domain classes
//
class BaseDom {
  var _domCnt$: sync int = 0; // domain reference count and lock
  var _arrs: list(BaseArr);   // arrays declared over this domain

  def getBaseDist(): BaseDist {
    return nil;
  }

  pragma "dont disable remote value forwarding"
  def destroyDom(arr: BaseArr = nil) {
    var cnt = _domCnt$ - 1;
    if cnt < 0 then
      halt("domain reference count is negative!");
    if arr then
      on arr do
        _arrs.remove(arr);
    _domCnt$ = cnt;
    if cnt == 0 {
      var dist = getBaseDist();
      if dist then on dist {
        var cnt = dist.destroyDist(this);
        if cnt == 0 then
          delete dist;
      }
    }
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
  def requiresPrivatization() param return false;

  // false for default distribution so that we don't increment the
  // default distribution's reference count and add domains to the
  // default distribution's list of domains
  def linksDistribution() param return true;
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

//
// Abstract array class
//
pragma "base array"
class BaseArr {
  var _arrCnt$: sync int = 0; // array reference count (and eventually lock)
  var _arrAlias: BaseArr;     // reference to base array if an alias

  def canCopyFromDevice param return false;
  def canCopyFromHost param return false;

  def getBaseDom(): BaseDom {
    return nil;
  }

  pragma "dont disable remote value forwarding"
  def destroyArr(): int {
    var cnt = _arrCnt$ - 1;
    if cnt < 0 then
      halt("array reference count is negative!");
    _arrCnt$ = cnt;
    if cnt == 0 {
      if _arrAlias {
        on _arrAlias {
          var cnt = _arrAlias.destroyArr();
          if cnt == 0 then
            delete _arrAlias;
        }
      } else {
        destroyData();
      }
      var dom = getBaseDom();
      on dom {
        var cnt = dom.destroyDom(this);
        if cnt == 0 then
          delete dom;
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
  def requiresPrivatization() param return false;
}
