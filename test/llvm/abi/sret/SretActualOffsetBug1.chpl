// BUG: When generating the body of an exported function, under LLVM, if the
// function has an indirect return (e.g. it returns a struct), Clang ABI
// support functions would map Chapel formals to the wrong LLVM formal (off
// by one error).
extern {
  // For the 'chpl_byte_buffer' extern struct def.
  #include "chpl-export-wrappers.h"

  // The LVT entry for this function will be pre-populated by this decl.
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

