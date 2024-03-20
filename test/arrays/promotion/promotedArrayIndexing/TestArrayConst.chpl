var ArrayOfIndices = [1, 3, 4, 3];
const D = {1..4};

const ArrayOfInt: [D] int;

record R {
  var val: int;
  proc ref addOne() do val += 1;
  proc print() do writeln(val);
}

const ArrayOfRecord: [D] R;
