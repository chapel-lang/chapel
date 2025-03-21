
use CTypes;

require "returnWide.h", "returnWide.c";

extern record data {
  var x : c_int;
}

extern proc getNewData() ref : data;
extern proc cleanup() : void;

ref d : data = getNewData();
writeln("before: ", d, ":: on ", d.locale);

on Locales[numLocales-1] {
  d = getNewData();
}

writeln("after: ", d, ":: on ", d.locale);

cleanup();
