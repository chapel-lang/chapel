//
// Example of a low level buffer that wraps _ddata. See #16797.
//

record buffer {
  type eltType;

  // TODO: How to make "on this" equivalent to "on this._data"?
  var _data: _ddata(eltType);
  var _size: int;

  proc init(type eltType, size: int) {
    this.eltType = eltType;
    if boundsChecking && size <= 0 {
      import HaltWrappers.boundsCheckHalt;
      boundsCheckHalt('Cannot allocate buffer with size <= 0');
    } else {
      this._data = _ddata_allocate(eltType, size, initElts=false);
      this._size = size;
    }
  }

  proc deinit() {
    _ddata_free(_data, _size);
  }

  inline proc size { return _size; }

  proc this(idx: int) ref {
    import HaltWrappers.boundsCheckHalt;
    if boundsChecking && (idx < 0 || idx >= _size) then
      boundsCheckHalt('Index \'' + idx:string + '\' is out of bounds');
    return _data[idx];
  }

  // A standard implementation may not want to offer this.
  iter these() ref {
    for i in 0..<_size do yield this[i];
  }

  // A standard implementation probably shouldn't offer this.
  proc writeThis(ch) {
    for slot in this do ch <~> slot <~> " ";
  }
}

