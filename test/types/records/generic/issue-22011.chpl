record generic_record {
    var x;
}

record wrapper_record {
    type t;
}

record R {
    var field: wrapper_record(generic_record);
}
