use CTypes;

extern proc fopen(filename: c_ptrConst(c_uchar), mode: c_ptrConst(c_uchar)): c_ptr(c_FILE);
extern proc fwrite(ptr: c_ptrConst(c_uchar), size: c_size_t, count: c_size_t, stream: c_ptr(c_FILE)): c_size_t;
extern proc fclose(stream: c_ptr(c_FILE)): c_int;

var f = fopen(c_ptrToConst("./test.txt"), c_ptrToConst("w+"));
if f == nil {
    writeln("failed to open file");
    halt(1);
}

var s = "Hello, World!\n";
fwrite(c_ptrToConst(s), 1, s.numBytes, f);
fclose(f);
