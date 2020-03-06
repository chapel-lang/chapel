// various utilites for testing

use DimensionalDist2D;

private use IO;


/// label the output with "phases" //////////////////////////////////////////

var pfx = "# ";
var phase = 0;
var no_pfx = false;

// to preserve coarse ordering ("phases") when sorting the output
proc nextphase() {
  phase += 1;
  pfx = if no_pfx then "" else "%{####}".format(phase) + "  ";
  traceDimensionalDistPrefix = pfx;
}

// Force the next phase to be 'ph' - so that when phases are added earlier,
// downstream output does not change.
// It is OK to advance to (the current phase number + 1), or later.
proc fphase(ph) {
  if phase >= ph {
    hd("###### advancing phase to a duplicate phase number: ",
       phase, " -> ", ph);
    tl();
  }
  phase = ph - 1;
}

// heading/start new "phase"
proc hd(args...) { nextphase(); writeln(pfx, (...args)); }
proc hhd(args...) { nextphase(); writeln(pfx, (...args), " on ", here.id); }
// additional messages
proc msg(args...) { writeln(pfx, (...args)); }
proc msgp() { write(pfx); }
proc msgserial(arg) { write(pfx); for e in arg do write(" ", e); writeln(); }
// tail - finish a "phase"
proc tl()         { writeln(pfx); }


/// locales for testing /////////////////////////////////////////////////////

var mylocdom: domain(2);
var mylocs: [mylocdom] locale?;
var manylocs: bool;

proc postfix!(A:[]) { var B = for a in A do a!; return B; } //#15080

// Initialize 'mylocs' and 'manylocs'.
//
proc setupLocales(s1:int, s2:int, ensureManyLocs: bool = false) {
  hd("setupLocales ", s1, "*", s2);
  mylocdom = {0..#s1,0..#s2};
  manylocs = (numLocales >= mylocs.numElements);

  if manylocs {
    var i = 0;
    for ml in mylocs { ml = Locales(i); i += 1; }
  } else {
    msg("oversubscribing Locales(0)");
    mylocs = Locales(0);
  }

  if !manylocs && ensureManyLocs then halt("not enough locales: wanted ",
    mylocs.numElements, ", got ", numLocales);

  msg("mylocs");
  msgserial(mylocs);
  tl();
}

// If oversubscribing a single locale, yield that one.
// Otherwise yield each of the distinct locales.
//
iter overLocales(): locale {
  if (manylocs) then
    for l in mylocs do yield l!;
  else
    yield here;
}

/// testing functions ///////////////////////////////////////////////////////
/// these are really a part of testing, so may change frequently

proc i2lTest(dm, ix) {
  hd(ix, " is on locale:");
  for l in overLocales() do
    on l do
      msg(l, ": ", dm.idxToLocale(ix));
  tl();
}

proc showDom(dmdom) {
  for l in overLocales() do
    on l do
      doShowDom(dmdom);
}

proc doShowDom(dmdom) {
  hhd("dmdom - serial iteration");
  msgserial(dmdom);
  tl();

  hhd("dmdom - parallel iteration");
  forall i in dmdom do msg(" ", i, "  on ", here.id);
  tl();
}

proc showArr(dmdom, dmarr, dmhelp) {
  for l in overLocales() do
    on l do
      doShowArr(dmdom, dmarr, dmhelp);
}

proc doShowArr(dmdom, dmarr, dmhelp) {
  hhd("dmarr - serial iteration");
  msgserial(dmarr);
  tl();

  hhd("dmarr - parallel iteration");
  forall elm in dmarr do
    msg(" ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr - assignment to dmhelp");
  resetDmhelp(dmhelp);
  dmhelp = dmarr;
  tl();
  
  hhd("dmhelp after assignment");
  msgserial(dmhelp);
  tl();

  //writeln(dmarr); -- on Locale 0
  //dmarr.writeThis(stdout); writeln(); -- works by chance

  hhd("dmdom, dmarr - zippered iteration");
  forall (ix,elm) in zip(dmdom,dmarr) do
    msg(" ", ix, " ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr, dmdom - zippered iteration");
  forall (elm,ix) in zip(dmarr,dmdom) do
    msg(" ", ix, " ", elm, "  eon ", elm.locale.id, "  on ", here.id);
  tl();

  hhd("dmarr, dmhelp - zippered iteration");
  resetDmhelp(dmhelp);
  forall (elm,hlp) in zip(dmarr,dmhelp) do
    msg(" ", elm, " ", hlp, "  on ", here.id);
  tl();
}

proc resetDmhelp(dmhelp: []) {
  if dmhelp.rank != 2 then
    compilerError("resetDmhelp is implemented only for 2-d arrays");

  for (ix, elm) in zip(dmhelp.domain, dmhelp) do
    elm = ix(1)*100 + ix(2);
}

var privTestWarningShown:bool = false;

// NOTE: this does not work because 'dmarr' is not treated as local
// in the 'on' clause
//
proc privTest(dmarr, locIds, arrIdx, expVal) {
  proc warn(msgtext) {
    if privTestWarningShown then return;
    privTestWarningShown = true;
    msg(msgtext);
  }

  if manylocs {
    if traceDimensionalDist {
      warn("skipped because traceDimensionalDist = true");

    } else {
      on mylocs(locIds) {
        msg(" mylocs", locIds, "  dmarr", arrIdx, "  expecting ", expVal,
            "  on ", here.id);
        var temp = 999999;
        const idx = arrIdx;
        assert(temp != expVal);
        local {
          temp = dmarr(idx);
        }
        if temp != expVal then
          msg("*** error: expected ", expVal, ", got ", temp);

      }
    }
  } else {
    warn("skipped because of oversubscribing Locales(0)");
  }
}
