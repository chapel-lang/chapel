module getACFile {
    require "./cf.h";

    use IO;
    use CTypes;

    extern proc openTestFile(): c_FILE;

    try! {
        var f = openfp(openTestFile(), hints = ioHintSet.fromFlag(QIO_HINT_OWNED));
        var r = f.reader();
        writeln(r.readLine());
    }
}

// module cCode {
//     extern {
//         #include <stdio.h>

//         static FILE* gimmeAFile() {
//             return fopen("./test/txt", "r");
//         }
//     }
// }

// var f = openfp(cCode.gimmeAFile():c_FILE, hints = ioHintSet.fromFlag(QIO_HINT_OWNED));
// var r = f.reader();

// writeln(r.readLine());
