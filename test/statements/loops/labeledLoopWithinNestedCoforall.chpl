coforall loc in Locales[0..0] do on loc {
    coforall tid in 0..<1 {
      label outer while true {
        var i = 1;
        do {
          writeln(i);
          if i == 3 then
            break outer;
          i += 1;
        } while (i < 100);
      }
    }
}
