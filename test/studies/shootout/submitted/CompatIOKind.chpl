use IO;

type iokind = _iokind;
proc type iokind.native param do return _iokind.native;

@deprecated("'ionative' is deprecated, please use Serializers or Deserializers that support endianness instead");
const ionative = iokind.native;

@deprecated("reader with a 'kind' argument is deprecated, please use Serializers instead");
proc file.reader(kind: iokind, param locking: bool, hints = ioHintSet.empty) {
  compilerWarning("'iokind' is deprecated, please use Serializers or Deserializers that support endianness instead");
  return this.reader(locking=locking, hints=hints, deserializer = new binaryDeserializer());
}

@deprecated("writer with a 'kind' argument is deprecated, please use Serializers instead");
proc file.writer(kind: iokind, param locking: bool, hints = ioHintSet.empty) {
  compilerWarning("'iokind' is deprecated, please use Serializers or Deserializers that support endianness instead");
  var fw = this.writer(locking=locking, hints=hints, serializer=none),
      st = fw._styleInternal();
  st.binary = 1;
  st.byteorder = kind:uint(8);
  fw._set_styleInternal(st);
  return fw;
}

proc _array.writeThis(f) {
  _value.dsiSerialWrite(f);
}
