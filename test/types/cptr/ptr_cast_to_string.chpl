use CTypes;
use Regex;

var mem = allocate(uint(8), 4096, clear=true);
if mem != c_nil then
  assert((mem:string).startsWith(new regex("0x[0-9a-f]+")));
var ptr = mem:c_void_ptr;
assert((mem:string) == (ptr:string));
deallocate(mem);
