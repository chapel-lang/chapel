

  record r {
    var atomicVar : if hasABASupport then _ddata(_ABAInternal(objType?)) else atomic uint(64);
    var _impl : unmanaged DistributedBagImpl(eltType)?;
  }
