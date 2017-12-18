
var redvar = 0;

forall lol in Locales with (
    + reduce redvar,
    var xxx = redvar + 1)
do
  redvar += lol.id;
