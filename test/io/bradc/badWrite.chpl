use IO;

const stdout = (new file(1)).writer(serializer=new binarySerializer(), locking=false);

// Historically this test existed to ensure that writing in native binary mode
// was not different betwen 'write' and 'writef'. In particular, this test
// was written to make sure a string length was not written at the beginning
// of the string.
//
// Now that we have Serializers and more direct ways to write binary data, this
// test exists to demonstrate the *difference* between 'write' and 'writef'.
//
// By default, the BinarySerializer is expected to emit a length before the
// string.
stdout.write(">foo bar\n");
stdout.writef("%s", ">foo bar\n");
