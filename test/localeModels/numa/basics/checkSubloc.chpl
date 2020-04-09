extern proc chpl_task_getRequestedSubloc(): chpl_sublocID_t;

for loc in Locales do on loc {
  if chpl_task_getRequestedSubloc() != c_sublocid_any then
    writeln("[", here.id,
            "] Wrong subloc (wanted ", c_sublocid_any,
            ", got ", chpl_getSubloc(), ")");

  for i in 0..#here.getChildCount() {
    on here.getChild(i) do
      if i!=chpl_getSubloc() then
        writeln("[", here.id,
                "] Wrong subloc (wanted ", i,
                ", got ", chpl_getSubloc(), ")");
  }
}

