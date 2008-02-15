enum ingredients {molasses, ginger, nutmeg, cloves, cinnamon};
config var gingerbread: ingredients = ingredients.molasses;

writeln("gingerbread (of type ingredients) is initialized at compile time to:");
writeln("molasses");
writeln("gingerbread at runtime is: ", gingerbread);
