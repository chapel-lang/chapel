use IO;
use CTypes;

testCptrToArray(openWriter("./c_ptr_out/cptr8.bin"), 8);
testCptrToArray(openWriter("./c_ptr_out/cptr16.bin"), 16);
testCptrToArray(openWriter("./c_ptr_out/cptr32.bin"), 32);
testCptrToArray(openWriter("./c_ptr_out/cptr64.bin"), 64);

proc testCptrToArray(writer, param isize: int) {
    var a = [0,1,2,3,4,5,6,7,8,9] : uint(isize);
    var p : c_ptr(uint(isize)) = c_ptrTo(a);
    writer.writeBinary(p, 10 * (isize / 8));
}
