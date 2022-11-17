use IO;
use CTypes;

readCptrTest(openreader("./c_ptr_input/cptr8.bin"), 8);
readCptrTest(openreader("./c_ptr_input/cptr16.bin"), 16);
readCptrTest(openreader("./c_ptr_input/cptr32.bin"), 32);
readCptrTest(openreader("./c_ptr_input/cptr64.bin"), 64);

// read 0..9 from the given file, expecting the given integer size,
//  and ensure that the correct values were read
proc readCptrTest(reader, param isize: int) {
    var cm = c_malloc(uint(isize), 10);
    var cm_void = cm : c_void_ptr;
    var num_read = reader.readBinary(cm_void, 10 * (isize / 8));
    var correct = true;
    for i in (0:uint(8))..<10 do correct &= cm[i] == i;
    writeln(num_read, "\t", correct);
    c_free(cm);
}

readCptrTestTooMany(openreader("./c_ptr_input/cptr8.bin"), 8);
readCptrTestTooMany(openreader("./c_ptr_input/cptr16.bin"), 16);
readCptrTestTooMany(openreader("./c_ptr_input/cptr32.bin"), 32);
readCptrTestTooMany(openreader("./c_ptr_input/cptr64.bin"), 64);

// try expecting a larger number of values than are present in the file.
//  ensure that the first 10 values were read correctly, and that 'num_read' is correct
proc readCptrTestTooMany(reader, param isize: int) {
    var cm = c_malloc(uint(isize), 12);
    var cm_void = cm : c_void_ptr;
    var num_read = reader.readBinary(cm_void, 12 * (isize / 8));
    var correct = true;
    for i in (0:uint(8))..<10 do correct &= cm[i] == i;
    writeln(num_read, "\t", correct);
    c_free(cm);
}
