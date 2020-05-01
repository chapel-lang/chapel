// this test was added to test promoted calls on the Locales array. `locale`
// record forwards to `BaseLocale`, however, at the time of adding this test
// forwarding doesn't work in promoted expressions. See 15148
writeln(Locales.numPUs());
writeln(Locales.id);
writeln(Locales.localeid);
writeln(Locales.hostname);
writeln(Locales.name);
writeln(Locales.chpl_id());
writeln(Locales.chpl_localeid());
writeln(Locales.chpl_name());
writeln(Locales.defaultMemory());
writeln(Locales.largeMemory());
writeln(Locales.lowLatencyMemory());
writeln(Locales.highBandwidthMemory());
writeln(Locales.getChildCount());
