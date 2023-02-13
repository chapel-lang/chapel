// a tricky case brought up in reviewing PR #21510

proc string.method() {
    writeln("Called on a string: ", this);
}

proc int.method() {
    writeln("Called on an int: ", this);
}

record GlobalRecord {
    var x = "hello";
    forwarding ((lambda (){
        record LocalRecord {
            var x = 42;

            proc methodThatUsesThis() {
                return x;
            }
        }
        var localRecord: LocalRecord;
        return localRecord.methodThatUsesThis();
    })());
}
var r: GlobalRecord;
r.method();
