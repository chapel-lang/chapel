use IO;

on Locales[0] {
    var w = open("./bs.bin", iomode.cw).writer();

    on Locales[1] {
        var d = heapAllocatedString(5);

        on Locales[2] {
            w.writeBinary(d);
        }
    }
}

proc heapAllocatedString(n: int) {
    return "the number " + n:string + " 👍\n";
}
