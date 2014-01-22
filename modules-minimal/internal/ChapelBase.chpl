// ChapelBase.chpl
//

pragma "no use ChapelStandard"
module ChapelBase {

  // Because resolution prefers user-defined versions to ones marked as "compiler
  // generated", it is desirable to add that flag to this default version.
  // In that way, a user-supplied version of assignment will override this one.
  pragma "compiler generated"
  inline proc =(a, b) return b;
  
  //
  // internal reference type
  //
  pragma "ref"
  pragma "no default functions"
  pragma "no object"
  class _ref {
    var _val;
  }

  pragma "compiler generated"
  pragma "init copy fn"
  inline proc chpl__initCopy(a) {
    // Currently, string representations are shared.
    // (See note on proc =(a:string, b:string) above.)
      return a;
  }
  
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x) return chpl__initCopy(x);
  
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(x: ?t) {
    __primitive("call destructor", x);
  }
}
