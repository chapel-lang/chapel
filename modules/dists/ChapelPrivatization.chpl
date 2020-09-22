
module ChapelPrivatization {

  private use CPtr;

  // the type of elements in chpl_privateObjects.
  pragma "no doc"
  extern record chpl_privateObject_t {
    var obj:c_void_ptr;
  }

  pragma "no doc"
  extern var chpl_privateObjects:c_ptr(chpl_privateObject_t);

  pragma "no doc"
  pragma "fn returns infinite lifetime"
  // should this use pragma "local args"?
  // Why is the compiler making the objectType argument wide?
  inline
  proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType {
    return __primitive("cast", objectType, chpl_privateObjects[objectPid].obj);
  }

}
