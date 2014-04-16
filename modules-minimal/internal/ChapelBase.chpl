// ChapelBase.chpl
//

pragma "no use ChapelStandard"
module ChapelBase {

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
