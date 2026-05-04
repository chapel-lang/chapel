use ChapelDynamicLoading;
use CommDiagnostics;
use CTypes;

/** Define a "user facing" wrapper around the internal dynamic library. */
record dynamicLibrary {
  var _bin: unmanaged chpl_BinaryInfo?;
  var _err: owned DynLoadError?;

  proc init(path: string) {
    init this;
    this._bin = chpl_BinaryInfo.create(path, this._err);
    if _bin then _bin!.bumpRefCount();
  }

  proc deinit() {
    if _bin then _bin!.dropRefCount();
  }

  proc err() do return _err.borrow();

  inline proc _assertRetrievedProcedureProperties(p1: ?t1) {
    use Types;

    compilerAssert(isProcedureValue(p1));
    compilerAssert(isProcedureType(t1));
    compilerAssert(isProcedure(p1));
    compilerAssert(isProcedure(t1));

    compilerAssert(!chpl_isLocalProc(p1));
    compilerAssert(!chpl_isLocalProc(t1));
    compilerAssert(!chpl_isLocalProcType(t1));

    compilerAssert(chpl_isWideProc(p1));
    compilerAssert(chpl_isWideProc(t1));
    compilerAssert(chpl_isWideProcType(t1));

    compilerAssert(chpl_isExternProc(p1));
    compilerAssert(chpl_isExternProc(t1));
    compilerAssert(chpl_isExternProcType(t1));

    const p2 = chpl_toLocalProc(p1);
    type t2 = p2.type;

    compilerAssert(isProcedureValue(p2));
    compilerAssert(isProcedureType(t2));
    compilerAssert(isProcedure(p2));
    compilerAssert(isProcedure(t2));

    compilerAssert(chpl_isLocalProc(p2));
    compilerAssert(chpl_isLocalProc(t2));
    compilerAssert(chpl_isLocalProcType(t2));

    compilerAssert(!chpl_isWideProc(p2));
    compilerAssert(!chpl_isWideProc(t2));
    compilerAssert(!chpl_isWideProcType(t2));

    compilerAssert(chpl_isExternProc(p2));
    compilerAssert(chpl_isExternProc(t2));
    compilerAssert(chpl_isExternProcType(t2));
  }

  proc ref load(sym: string, type T) throws {
    if _err != nil then throw _err;
    var err;
    var ret = _bin!.loadSymbol(sym, T, err);
    if err != nil then throw err;
    _assertRetrievedProcedureProperties(ret);
    return ret;
  }
}
