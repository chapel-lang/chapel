record generic_record {
    var x;
}

record wrapper_record {
    type t;
}

record R {
    var field: wrapper_record(generic_record(?));
}

var gr = new generic_record(x=10);
writeln(gr, ": ", gr.type:string);
var wrap = new wrapper_record(gr.type);
writeln(wrap, ": ", wrap.type:string);
var r = new R(field = wrap);
writeln(r, ": ", r.type:string);
