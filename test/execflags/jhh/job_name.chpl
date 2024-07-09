use CTypes;
extern proc sys_getenv(name:c_ptrConst(c_char), ref string_out:c_ptrConst(c_char)):c_int;

var value_str:c_ptrConst(c_char);
var value:string;
if sys_getenv(c"CHPL_LAUNCHER_JOB_PREFIX", value_str)==1 {
    value = string.createCopyingBuffer(value_str);
} else {
    value = "NONE";
}

writeln("CHPL_LAUNCHER_JOB_PREFIX: ", value);

if sys_getenv(c"CHPL_LAUNCHER_JOB_NAME", value_str)==1 {
    value = string.createCopyingBuffer(value_str);
} else {
    value = "NONE";
}

writeln("CHPL_LAUNCHER_JOB_NAME: ", value);

config var test = 0;
