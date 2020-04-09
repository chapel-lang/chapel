use Sys;

const ENV_VAR = c"QQ62ZZ21Q";

proc main()
{
  var foo: c_string;

  if sys_getenv(ENV_VAR, foo)
  {
    writeln("found $", createStringWithNewBuffer(ENV_VAR), " = ",
                       createStringWithNewBuffer(foo));
    exit(0);
  }
  else
  {
    writeln("failed to find $", createStringWithNewBuffer(ENV_VAR));
    exit(1);
  }

}

