use IO;

on Locales[0] {
    var w = open("./bb.bin", ioMode.cw).writer();

    on Locales[1] {
        var d = heapAllocatedBytes(5);

        on Locales[2] {
            w.writeBinary(d);
        }
    }
}

proc heapAllocatedBytes(n: int) {
    return b"the number " + n:bytes + b" \xF0\x9F\x91\x8D\n";
}
