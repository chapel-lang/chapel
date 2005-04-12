enum ingredients {molasses, ginger, nutmeg, cloves, cinnamon};
config var gi?ngerbread: ingredients = molasses;

writeln("gi?ngerbread (of type ingredients) is initialized at compile time to:");
writeln("molasses");
writeln("gi?ngerbread at runtime is: ", gi?ngerbread);
