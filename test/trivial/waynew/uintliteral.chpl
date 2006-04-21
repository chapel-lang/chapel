var i:int;
var ui:uint;

ui = 9u;
writeln ("ui=9u is ", ui);
ui = 9U;
writeln ("ui=9U is ", ui);
i = ui;
writeln ("i=ui is ", i);
i = 12u;
writeln ("i=12u is ", i);
ui = 18446744073709551593u;
i = ui;
writeln ("i=ui is ", i);
i = 18446744073709551593u;
writeln ("i=18446744073709551593u is ", i);

