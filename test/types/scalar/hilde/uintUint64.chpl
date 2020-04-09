// uintUint64.chpl
//
// Allow operations between uint(32) and uint64 to proceed.
//
// This involves changing the promotion path, so that uint32 is promoted to uint64, not int64.
//

var u : uint(32) = 0;
var v : uint(64) = 1;

writeln((u+v).type:string);
