/** Git Intro!
 * 
 * The Chapel team's intro to git version control. The core team members should
 * update their name tuple in the array below.
 */

// To play Chapel adlibs, fill out your tuple with a programming language, a
// feeling, and a verb.
//
// name
//         programming language
//                 feeling
//                         verb
const TheTeam = [
  ("Ben", "C", "productive", "enjoy"),
  ("Brad", "xfig", "liberated", "begat"),
  ("David", "", "", ""),
  ("Greg", "snobol", "much less strung out", "really dig"),
  ("Kyle", "C", "well", "like"),
  ("Lydia", "Java", "pleasantly intrigued", "poke"),
  ("Michael", "", "", ""),
  ("Elliot", "", "", ""),
  ("Simon", "", "", ""),
  ("Sung", "", "", ""),
  ("Thomas", "Python", "ecstatic", "<3"),
  ("Tim", "C", "free", "sorta kinda like"),
  ("TMac", "", "", ""),
  ("Tom", "", "", ""),
  ("Vass", "a hammer", "empowered", "git")
];

// Prologue
writeln("Chapel Adlibs!");
writeln("--------------");
writeln();

// Print the adlibs!
for (name, lang, feeling, verb) in TheTeam {
  writef("My name is %s. I used to use %s for all my problems.\nNow, I use Chapel and feel %s! I %s Chapel.\n",
         name, lang, feeling, verb);
  writeln();
}
