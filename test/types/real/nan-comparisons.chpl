var nan = NAN;
var inf = INFINITY;
var neginf = -INFINITY;

writeln("isnan(nan) ", isnan(nan));

writeln("nan >= 0.0 ", nan >= 0.0);
writeln("nan <= 0.0 ", nan <= 0.0);
writeln("nan > 0.0 ", nan > 0.0);
writeln("nan < 0.0 ", nan < 0.0);
writeln("nan == 0.0 ", nan == 0.0);
writeln("nan != 0.0 ", nan != 0.0);

writeln("nan >= inf ", nan >= inf);
writeln("nan <= inf ", nan <= inf);
writeln("nan > inf ", nan > inf);
writeln("nan < inf ", nan < inf);
writeln("nan == inf ", nan == inf);
writeln("nan != inf ", nan != inf);

writeln("nan >= neginf ", nan >= neginf);
writeln("nan <= neginf ", nan <= neginf);
writeln("nan > neginf ", nan > neginf);
writeln("nan < neginf ", nan < neginf);
writeln("nan == neginf ", nan == neginf);
writeln("nan != neginf ", nan != neginf);

writeln("nan >= nan ", nan >= nan);
writeln("nan <= nan ", nan <= nan);
writeln("nan > nan ", nan > nan);
writeln("nan < nan ", nan < nan);
writeln("nan == nan ", nan == nan);
writeln("nan != nan ", nan != nan);



writeln("isinf(inf) ", isinf(inf));
writeln("inf >= 0.0 ", inf >= 0.0);
writeln("inf <= 0.0 ", inf <= 0.0);
writeln("inf > 0.0 ", inf > 0.0);
writeln("inf < 0.0 ", inf < 0.0);
writeln("inf == 0.0 ", inf == 0.0);
writeln("inf != 0.0 ", inf != 0.0);

writeln("inf >= inf ", inf >= inf);
writeln("inf <= inf ", inf <= inf);
writeln("inf > inf ", inf > inf);
writeln("inf < inf ", inf < inf);
writeln("inf == inf ", inf == inf);
writeln("inf != inf ", inf != inf);

writeln("inf >= neginf ", inf >= neginf);
writeln("inf <= neginf ", inf <= neginf);
writeln("inf > neginf ", inf > neginf);
writeln("inf < neginf ", inf < neginf);
writeln("inf == neginf ", inf == neginf);
writeln("inf != neginf ", inf != neginf);



writeln("isinf(neginf) ", isinf(neginf));
writeln("neginf >= 0.0 ", neginf >= 0.0);
writeln("neginf <= 0.0 ", neginf <= 0.0);
writeln("neginf > 0.0 ", neginf > 0.0);
writeln("neginf < 0.0 ", neginf < 0.0);
writeln("neginf == 0.0 ", neginf == 0.0);
writeln("neginf != 0.0 ", neginf != 0.0);
