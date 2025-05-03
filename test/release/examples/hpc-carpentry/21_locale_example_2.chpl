for loc in Locales do   // This is still a serial program
  on loc do             // run the next line on locale `loc`
    writeln("This locale is named ", here.name);
