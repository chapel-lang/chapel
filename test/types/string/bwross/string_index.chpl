var s:string = "hello world";

// string_index_of
write(s.indexOf(" "), " ");
write(s.indexOf("hello"), " ");
write(s.indexOf("world"), " ");
write(s.indexOf("nowhere"), " ");
write(s.indexOf(s), " ");
writeln(s.indexOf(""));

// string_select
writeln(s.substring(..));
writeln(s.substring(..5));
writeln(s.substring(7..));
writeln(s.substring(4..8));
writeln(s.substring(-50..50));
writeln(s.substring(-100..-50));
writeln(s.substring(50..100));

// with stride
writeln(s.substring(1..11 by 2));
writeln(s.substring(1..11 by -1));
writeln(s.substring(1..11 by -2));
writeln(s.substring(1.. by 3));
writeln(s.substring(2.. by 3));
writeln(s.substring(-3.. by 3));
writeln(s.substring(-1.. by 3));
writeln(s.substring(-2.. by 3));

// string_index cases
writeln(s.substring(99));
writeln(s.substring(3));
writeln(s.substring(-30));

// all together now!
writeln(s.substring(1..s.indexOf(" ")),
        "beautifu", s.substring(3),
        s.substring(s.indexOf(" ")..));
