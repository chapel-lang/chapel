// Chapel file that exports a function that operates in a distributed fashion
// Adjusted from test/release/examples/hello6-taskpar-dist.chpl
export proc foo(tasksPerLocale: int) {
  coforall loc in Locales {
    on loc {
      coforall tid in 0..#tasksPerLocale {
        var message = "Hello, world! (from ";

        if (tasksPerLocale > 1) then
          message += "task " + tid: string + " of " + tasksPerLocale: string + " on ";

        message += "locale " + here.id: string + " of " + numLocales: string;

        message += ")";
        writeln(message);
      }
    }
  }
}
