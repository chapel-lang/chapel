use CTypes;

extern proc fopen(filename: c_ptrConst(c_char), mode: c_ptrConst(c_char)): c_FILE;
extern proc fwrite(ptr: c_ptrConst(c_char), size: c_size_t, count: c_size_t, stream: c_FILE): c_size_t;
extern proc fclose(stream: c_FILE): c_int;

var f = fopen("./filePtrTest.txt".c_ptr_c_char(), "w+".c_ptr_c_char());
var s = "Hello, World!\n";
fwrite(s.c_ptr_c_char(), 1:c_size_t, s.numBytes:c_size_t, f);
fclose(f);
