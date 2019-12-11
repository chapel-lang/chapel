import stringAllocated
import numpy

stringAllocated.chpl_setup()
x = numpy.empty(6, dtype=numpy.uint8)
newsize = stringAllocated.g(x.size, x)
if (newsize is not -1):
    x = numpy.delete(x, range(newsize, x.size))
else:
    stringAllocated.writeStr("Buffer wasn't wide enough")
resString = ""
for character in x:
    resString += str(chr(character))
stringAllocated.writeStr(resString);
stringAllocated.chpl_cleanup()
