class C { }

// cast various memory flavors to class / class?

writeln(C  :class? :string);
writeln(C? :class  :string);

writeln(owned C  :class? :string);
writeln(owned C? :class  :string);

writeln(shared C  :class? :string);
writeln(shared C? :class  :string);

writeln(borrowed C  :class? :string);
writeln(borrowed C? :class  :string);

writeln(unmanaged C  :class? :string);
writeln(unmanaged C? :class  :string);
