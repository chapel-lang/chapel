use IO;
use CTypes;

testCptrToArray(openwriter("./c_ptr_out/cvoidptr8.bin"), 8);
testCptrToArray(openwriter("./c_ptr_out/cvoidptr16.bin"), 16);
testCptrToArray(openwriter("./c_ptr_out/cvoidptr32.bin"), 32);
testCptrToArray(openwriter("./c_ptr_out/cvoidptr64.bin"), 64);

proc testCptrToArray(writer, param isize: int) {
    var a = [0,1,2,3,4,5,6,7,8,9] : uint(isize);
    var p : c_ptr(uint(isize)) = c_ptrTo(a);
    var pv = p : c_void_ptr;
    writer.writeBinary(pv, 10 * (isize / 8));
}
