use OS.POSIX;
use Set;

var s = new set(int(32));

s.add(SIGABRT);
s.add(SIGALRM);
s.add(SIGBUS);
s.add(SIGCHLD);
s.add(SIGCONT);
s.add(SIGFPE);
s.add(SIGHUP);
s.add(SIGILL);
s.add(SIGINT);
s.add(SIGKILL);
s.add(SIGPIPE);
s.add(SIGQUIT);
s.add(SIGSEGV);
s.add(SIGSTOP);
s.add(SIGTERM);
s.add(SIGTRAP);
s.add(SIGTSTP);
s.add(SIGTTIN);
s.add(SIGTTOU);
s.add(SIGURG);
s.add(SIGUSR1);
s.add(SIGUSR2);
s.add(SIGXCPU);
s.add(SIGXFSZ);

writeln(s.size == 24);
