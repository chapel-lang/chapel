// Checks basic deprecated declarations of enums
deprecated enum colors {red, green, blue};
deprecated "stooges is deprecated, use marx instead"
enum stooges {curly, moe, larry};
enum marx {harpo, groucho, chico, zeppo};

// Checks interaction with documentation
/* There was documentation of this symbol */
deprecated enum atomicSymbols {proton, electron, neutron};
/* This symbol also was documented */
deprecated "monsters is deprecated, use monsterMash instead"
enum monsters {vampire, werewolf, ghost};
/* Truncated Frankenstein's monster */
enum monsterMash {Frankenstein, Wolfman, Dracula, Casper};

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is deprecated */
deprecated enum directions {north, south, east, west};
/* This symbol is also deprecated, please use bones instead */
deprecated "bodyParts is deprecated, use bones instead"
enum bodyParts {head, shoulders, knees, toes};
/* Replaces the deprecated bodyParts */
enum bones {tibia, fibia, skull, rib, kneecap};

// Ensures deprecation doesn't cause "no doc" symbols to turn up in
// documentation
pragma "no doc"
deprecated enum spiceGirls {posh, baby, scary, sporty, ginger};
pragma "no doc"
deprecated "nsync is so early 2000s"
enum nsync {justin, lance, jc, joey, chris};
