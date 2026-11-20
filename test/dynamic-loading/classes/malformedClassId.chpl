use CTypes;
import DynamicLibrary;
use newSubClassCommon;

require "chpltypes.h";
extern const CHPL_CID_IDX_BITS: c_int;

config const path = "./lib"
config const scenario = 0;

proc main() {
  var bin = DynamicLibrary.load("malformedClassIdLib", path);
  var callGetX = bin.retrieve("callGetX", proc(ptr: c_ptr(void)): int);
  var obj = new A(1);
  const ptr = c_ptrTo(obj);

  // Verify a valid class from the executable dispatches in the library.
  writeln(callGetX(ptr));

  // The class ID is the first field of a Chapel class instance.
  const cidPtr = ptr:c_ptr(uint(32));
  const cid = __primitive("getcid", obj.borrow());
  const indexMask = (1:uint(32) << CHPL_CID_IDX_BITS) - 1;
  select scenario {
    when 0 do cidPtr[0] = 1; // Reserved program ID zero.
    when 1 do cidPtr[0] = cid & ~indexMask;
    when 2 do cidPtr[0] = cid | indexMask;
    otherwise do halt("unknown test scenario");
  }

  writeln(callGetX(ptr));
  halt("class ID translation unexpectedly returned");
}
