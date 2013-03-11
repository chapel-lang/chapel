use Sys;

const ENV_VAR = "QQ62ZZ21Q";

proc main()
{
  var foo: string;

  if sys_getenv(ENV_VAR, foo)
  {
    //writeln("found $", ENV_VAR);
    exit(0);
  }
  else
  {
    //writeln("failed to find $", ENV_VAR);
    exit(1);
  }

}

