// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "cstrvoid.h";

// Note: Generated with fake std headers

extern proc arg_c_string(str : c_string, n : c_int) : void;

extern proc arg_c_void_ptr(ptr : c_void_ptr) : void;

extern proc ret_cstr() : c_string;

extern proc ret_void_ptr() : c_void_ptr;

extern proc arg_ptr_to_cstr(ref out : c_string) : void;

extern proc arg_ptr_to_void_ptr(ref out : c_void_ptr) : void;

