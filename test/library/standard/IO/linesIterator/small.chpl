use IO;

forall line in openReader("small.in").lines(targetLocales=Locales) do
    write("Locale ", here.id, " read: ", line);
