config const par = true;

serial par {
  serial !par do {
    var serialState = __primitive("task_get_serial");
    writeln(serialState);
  }
}

serial par {
  begin { serial !par do {
      var serialState = __primitive("task_get_serial");
      writeln(serialState);
    }
  }
}
