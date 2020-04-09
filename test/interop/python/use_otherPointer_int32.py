import otherPointer_int32
import ctypes

otherPointer_int32.chpl_setup()
x = ctypes.c_int32()
otherPointer_int32.writeInt(x.value)
y = ctypes.pointer(x)
otherPointer_int32.gimmePointer(y)
otherPointer_int32.writeInt(x.value)
otherPointer_int32.chpl_cleanup()
