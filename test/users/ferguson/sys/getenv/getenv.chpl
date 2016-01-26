use Sys;

const ENV_VAR = c"QQ62ZZ21Q";

proc main()
{
  var foo: c_string;

  if sys_getenv(ENV_VAR, foo)
  {
    writeln("found $", ENV_VAR:string, " = ", foo:string);
    exit(0);
  }
  else
  {
    writeln("failed to find $", ENV_VAR:string);
    exit(1);
  }

}

