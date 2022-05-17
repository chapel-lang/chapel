use Sys;

writeln("job_prefix starting");
var value_str:c_string;
var value:string;
if sys_getenv(c"CHPL_LAUNCHER_JOB_PREFIX", value_str)==1 {
    value = createStringWithNewBuffer(value_str);
} else {
    value = "NONE";
}

writeln("CHPL_LAUNCHER_JOB_PREFIX: ", value);

if sys_getenv(c"CHPL_LAUNCHER_JOB_NAME", value_str)==1 {
    value = createStringWithNewBuffer(value_str);
} else {
    value = "NONE";
}

writeln("CHPL_LAUNCHER_JOB_NAME: ", value);

config var test = 0;
