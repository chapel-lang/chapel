use Channel;

var chan1 = new channel(int);

begin {
    {
        cobegin {
            chan1.send(5);
            chan1.send(6);
            chan1.send(8);
            chan1.send(10);
        }
        chan1.close();
    }
}

var sum = 0;
for i in chan1 {
    sum += i;
}
writeln(sum == 29);
