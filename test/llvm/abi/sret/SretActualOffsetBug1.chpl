// BUG: When generating calls to exported functions, under LLVM, if the
// function has an indirect return (e.g. it returns a struct), the
// call emits garbage. This is because the loop that emits code for
// arguments does not adjust the LLVM arg offsets to account for the
// indirect return (which is the 0th formal).
extern {
  // For the 'chpl_byte_buffer' extern struct def.
  #include "chpl-export-wrappers.h"

  // The LVT for 'printFoo' will be pre-populated by this decl.
  chpl_byte_buffer takeAndReturnByteBuffer(chpl_byte_buffer someBuf);
}

export proc takeAndReturnByteBuffer(in someBuf: chpl_byte_buffer) {
  return someBuf;
}

proc test() {
  // ABI support should fire for this call (because it is an exported fn).
  var buf1 = new chpl_byte_buffer();
  var buf2 = takeAndReturnByteBuffer(buf1);
  writeln(buf1);
  writeln(buf2);
  assert(buf1 == buf2);
}
test();

