extern proc sys_gethostname(ref name: c_string): err_t;

var x: c_string;
sys_gethostname(x);
writeln(x: string);
