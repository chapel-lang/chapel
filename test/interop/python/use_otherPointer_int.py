import otherPointer_int
import ctypes

otherPointer_int.chpl_setup()
x = ctypes.c_int()
otherPointer_int.writeInt(x.value)
y = ctypes.pointer(x)
otherPointer_int.gimmePointer(y)
otherPointer_int.writeInt(x.value)
otherPointer_int.chpl_cleanup()
