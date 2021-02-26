use CPtr;
const myDom = {0..5};

record things {
  var a: int(64);
  var v: int(64);
}

extern record data_set {
}

extern proc read_stuff(location: c_ptr(data_set), stuff: [] things,
                    start: uint(64), size: uint(64), count: uint(64)): int(64);

proc get_stuff()
{
    var stuff: [myDom] things;
    var num: int(64);
    var location: c_ptr(data_set);

    num = read_stuff(location, stuff, 0, 16, 6);
    writeln(stuff);
}

proc main() {
    get_stuff();
}
