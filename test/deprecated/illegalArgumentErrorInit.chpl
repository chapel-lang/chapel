const iae1 = new IllegalArgumentError(info="bad argument!"),
      iae2 = new IllegalArgumentError(msg="bad argument!"),
      iae3 = new IllegalArgumentError("bad argument!");

writeln(iae1);
writeln(iae2);
writeln(iae3);

const iae4 = new IllegalArgumentError("formal_name", "bad!");
writeln(iae4);
