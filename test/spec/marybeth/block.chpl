var tau, s, c: real;
const a = 2.0;
const b = 5.5;
{
tau = -a/b;
s = 1/sqrt(1 + tau*tau);
c = s*tau;
}
writeln("Givens rotation = ", s, " ", c);
