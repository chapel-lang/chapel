extern proc chpl_task_getSubloc(): chpl_sublocID_t;
extern proc chpl_task_getRequestedSubloc(): chpl_sublocID_t;

for loc in Locales do on loc {
  if chpl_task_getRequestedSubloc() != c_sublocid_any then
    writeln("[", here.id,
            "] Wrong subloc (wanted ", c_sublocid_any,
            ", got ", chpl_task_getSubloc(), ")");

  for i in 0..#(here:LocaleModel).numSublocales do
    on (here:LocaleModel).getChild(i) do
      if i!=chpl_task_getSubloc() then
        writeln("[", here.id,
                "] Wrong subloc (wanted ", i,
                ", got ", chpl_task_getSubloc(), ")");
}

