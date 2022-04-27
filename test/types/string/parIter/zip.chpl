use CTypes;

enum iterTypes { default, dotItems, dotBytes };

config type dataType = string;
config param iterType = iterTypes.default;

const charRange = 33..126;

var buf = c_calloc(uint(8), charRange.size);
for i in charRange do buf[i-charRange.low] = i:uint(8);
var str = createStringWithOwnedBuffer(buf, charRange.size, charRange.size);

var rng = charRange;
var dom = {rng};

test((getIter(str), getIter(str)),               1);
test((getIter(str), rng),                        2);
test((getIter(str), dom),                        3);
test((rng,          getIter(str), dom),          4);
test((rng,          dom,          getIter(str)), 5);

proc test(zipTuple, id) {
  forall x in zip((...zipTuple)) do writeln(id, ": ", x);
}

proc getIter(s) {
  select iterType {
    when iterTypes.default do return s;
    when iterTypes.dotItems do return s.items();
    when iterTypes.dotBytes do return s.bytes();
    otherwise do compilerError("Unknown iterType");
  }
}
