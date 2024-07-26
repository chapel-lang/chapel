coforall i in 0..<here.gpus.size do on here.gpus[i] do assert(here == Locales[0].gpus[i]);

const locales = here.gpus;
coforall i in 0..<here.gpus.size do on locales[i] do assert(here == Locales[0].gpus[i]);

for (l, i) in zip(locales, locales.domain) do on l do assert(here == Locales[0].gpus[i]);
