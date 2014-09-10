use Sys;

const ENV_VAR = "QQ62ZZ21Q".c_str();

proc main()
{
  var foo: c_string;

  if sys_getenv(ENV_VAR, foo)
  {
    writeln("found $", ENV_VAR, " = ", foo);
    exit(0);
  }
  else
  {
    writeln("failed to find $", ENV_VAR);
    exit(1);
  }

}

