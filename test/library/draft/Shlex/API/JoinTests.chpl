use Shlex;
writeln(join(['a', 'b', 'c']));
writeln(join(('a', 'b', 'c')));
writeln(join('a', 'b', 'c'));
writeln(join(['echo', '-n', 'Multiple words']));
writeln(join(['echo', 'filename; rm -rf "home"']));
