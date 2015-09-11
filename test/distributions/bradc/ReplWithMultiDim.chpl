use ReplicatedDist;

const Space = {1..8, 1..8};
const OneLocOnly: [0..0, 0..0] locale = Locales[0];
const R2 = Space dmapped ReplicatedDist(targetLocales=OneLocOnly);
writeln(R2);
