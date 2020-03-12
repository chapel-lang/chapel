class C { }

// cast various memory flavors to C / C?

writeln(C  :C? :string);
writeln(C? :C  :string);

writeln(owned C  :C? :string);
writeln(owned C? :C  :string);

writeln(shared C  :C? :string);
writeln(shared C? :C  :string);

writeln(borrowed C  :C? :string);
writeln(borrowed C? :C  :string);

writeln(unmanaged C  :C? :string);
writeln(unmanaged C? :C  :string);
