record myRec {}

record container {
    var rec: myRec;

    proc init() {
        init this;
        rec.init();
    }
}

var rec = new myRec();
rec.init();
