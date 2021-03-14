/* bug: Per spec, exterior, interior, translate should take single int arg */

var d : domain(rank=2) = { 1..10, 11..20 };
writeln("exterior ", d.exterior(2));
writeln("interior ", d.interior(2));
writeln("translate ", d.translate(2));
