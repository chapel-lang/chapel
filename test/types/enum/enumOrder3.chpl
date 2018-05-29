enum color { red=42, green=22, blue=32};

writeln(color.red);

writeln(chpl__enumToOrder(color.red));
writeln(chpl__enumToOrder(color.green));
writeln(chpl__enumToOrder(color.blue));

writeln(chpl__orderToEnum(0, color));
writeln(chpl__orderToEnum(1, color));
writeln(chpl__orderToEnum(2, color));
writeln(chpl__orderToEnum(3, color));
