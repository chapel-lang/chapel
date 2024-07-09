use IO;

var fr = openReader("listInput.txt", locking=false);

test(fr, Locales[0], Locales[1]);
test(fr, Locales[1], Locales[0]);
test(fr, Locales[1], Locales[1]);
test(fr, Locales[1], Locales[2]);

proc test(fr, L1: locale, L2: locale) {
    on L1 {
        var s = "", b = b"";
        on L2 {
            fr.readThrough(",", s);
            fr.readThrough(b",", b);
        }
        writeln(s);
        writeln(b);
    }
}
