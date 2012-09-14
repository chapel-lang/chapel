// subloc2.chpl

extern proc chpl_task_getSubLoc() : int(32);
extern proc chpl_task_setSubLoc(subloc:int(32)) : void;

class SubLoc : locale
{
}

var mySubLoc : locale = nil;

on __primitive("chpl_on_locale_num", 0, 13) do
  mySubLoc = new SubLoc();

writeln("mySubLoc thinks it is on sublocale ",
        __primitive("_wide_get_subloc", mySubLoc));

on mySubLoc
{
  writeln("Now running on sublocale ", chpl_task_getSubLoc());
}