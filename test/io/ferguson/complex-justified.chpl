writef("|%10i|%-10i|\n", 1, 2);
writef("|%10i|%-10i|\n", -1, -2);
writef("|%10z|%-10z|\n", 1+1i, 2+2i);
writef("|%10z|%-10z|\n", -1-1i, -2-2i);
writef("|%@10z|%@-10z|\n", 1+1i, 2+2i);
writef("|%@10z|%@-10z|\n", -1-1i, -2-2i);
writef("|%10n|%-10n|\n", 1i, 2i);
writef("|%10n|%-10n|\n", -1i, -2i);

writef("right |%20r|\n", 1.234);
writef("left  |%-20r|\n", 1.234);
writef("right |%20z|\n", 1+2i);
writef("left  |%-20z|\n", 1+2i);
