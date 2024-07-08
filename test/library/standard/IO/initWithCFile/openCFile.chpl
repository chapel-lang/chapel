module m {
    require "cf.h";

    use IO;
    use CTypes;

    extern proc openTestFile(): c_ptr(c_FILE);

    try! {
        var f = new file(openTestFile(), own=true);
        var r = f.reader(locking=false);
        write(r.readLine());
    }
}
