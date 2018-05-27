enum color { red=42, green, blue};

writeln(color.red);

writeln(chpl__enumToOrder(color.red));
writeln(chpl__enumToOrder(color.green));
writeln(chpl__enumToOrder(color.blue));

writeln(chpl__orderToEnum(1, color));
writeln(chpl__orderToEnum(2, color));
writeln(chpl__orderToEnum(3, color));
writeln(chpl__orderToEnum(4, color));
