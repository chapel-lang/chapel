
use printfGen, CTypes;

printf_wrapper("%lld %s %lld\n".c_ptr_c_char(), 256, "hello world".c_ptr_c_char(), 42);
