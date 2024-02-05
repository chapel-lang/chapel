use CTypes;

require "ArrayWrappersUnknownTypeBug.h";

extern record Foo {
  var a : chpl_external_array;
  var b : chpl_opaque_array;
}

record Bar {
  var b : c_ptr(Foo);
}

proc main() {
  const x = new Bar(nil);
}
