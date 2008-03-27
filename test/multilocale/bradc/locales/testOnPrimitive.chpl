on __primitive("chpl_on_locale_num", 0) do writeln("on ", here.id);

for i in 0..numLocales-1 do
  on __primitive("chpl_on_locale_num", i) do writeln("on ", here.id);

