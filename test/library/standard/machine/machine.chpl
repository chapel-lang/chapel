use MemDiagnostics;  // This should not be necessary, but currently I cannot
             // put the definition of the Locales array into
             // _chpl_machine.chpl without breaking reallocation of
             // 1D uint arrays.

writeln("numLocales = ", numLocales);
writeln("LocaleSpace = ", LocaleSpace);
writeln("Locales = ", Locales);
