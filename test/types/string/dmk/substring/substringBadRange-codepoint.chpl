var str1 = "abcdefghijklm";

writeln("str1[12:byteIndex..13:byteIndex] = ", str1[12:byteIndex..13:byteIndex]);

var str2 = "åbçdèfghíjklm";

writeln("str2[12:codepointIndex..13:codepointIndex] = ", str2[12:codepointIndex..13:codepointIndex]);
writeln("str2[12:codepointIndex..16:codepointIndex] = ", str2[12:codepointIndex..16:codepointIndex]);
