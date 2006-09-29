var i:int(64);
var ui:uint(64);

ui = 9;
writeln ("ui=9u is ", ui);
ui = 9;
writeln ("ui=9U is ", ui);
i = ui:int;
writeln ("i=ui is ", i);
i = 12:int;
writeln ("i=12u is ", i);
ui = 18446744073709551593;
i = ui:int;
writeln ("i=ui is ", i);
i = 18446744073709551593:int;
writeln ("i=18446744073709551593u is ", i);

