use CTypes, IO;

// test that a user will be warned when that pass a raw c_FILE to a file initializer
// This will likely be a common mistake when transitioning from cFileTypeHasPointer==true to cFileTypeHasPointer==false

// this should return a c_ptr(c_FILE)
extern proc fopen(filename: c_ptrConst(c_uchar), mode: c_ptrConst(c_uchar)): c_FILE;
var f = new file(fopen(c_ptrToConst("warningTest.txt"), c_ptrToConst("w")));
