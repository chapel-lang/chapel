// see issue #24591

proc p(arg: range(bounds=?)) do
  writeln(arg, " : ", arg.type:string);

var r: range(strides=strideKind.negative);
p(r); // should be an error
