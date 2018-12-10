import otherPointer
import ctypes

otherPointer.chpl_setup()
x = ctypes.c_double()
otherPointer.writeReal(x.value)
y = ctypes.pointer(x)
otherPointer.gimmePointer(y)
otherPointer.writeReal(x.value)
otherPointer.chpl_cleanup()
