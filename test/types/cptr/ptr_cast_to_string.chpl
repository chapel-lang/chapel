use CTypes;
use Regex;

var mem = c_calloc(uint(8), 4096);
if mem != c_nil then
  assert((mem:string).startsWith(compile("0x[0-9a-f]+")));
var ptr = mem:c_void_ptr;
assert((mem:string) == (ptr:string));
c_free(mem);
