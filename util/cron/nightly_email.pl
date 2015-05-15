#!/usr/bin/env perl

use FindBin;
use lib "$FindBin::Bin";

use nightlysubs;

$num_args = $#ARGV + 1;
if ($num_args != 16) {
    print "usage: nightly_email.pl \$status \$rawsummary \$sortedsummary \n";
    print "         \$prevsummary \$mailer \$nochangerecipient \$recipient \n";
    print "         \$subjectid \$config_name \$revision \$rawlog \$starttime \n";
    print "         \$endtime \$crontab \$testdirs \$debug\n";
    exit 1;
}

$status = $ARGV[0];
$rawsummary = $ARGV[1];
$sortedsummary = $ARGV[2];
$prevsummary = $ARGV[3];
$mailer = $ARGV[4];
$nochangerecipient = $ARGV[5];
$recipient = $ARGV[6];
$subjectid = $ARGV[7];
$config_name = $ARGV[8];
$revision = $ARGV[9];
$rawlog = $ARGV[10];
$starttime = $ARGV[11];
$endtime = $ARGV[12];
$crontab = $ARGV[13];
$testdirs = $ARGV[14];
$debug = $ARGV[15];


# Ensure the "previous" summary exists, e.g. if this is the first run of the
# configuration they won't.
ensureSummaryExists($prevsummary);


#
# sort output
#

if ($status == 0) {
    `cat $rawsummary | grep -v "^.END" | grep -v "^.Test Summary" | LC_ALL=C sort > $sortedsummary`;

    $oldsummary = `grep Summary: $prevsummary`; chomp($oldsummary);
    $cursummary = `grep Summary: $sortedsummary`; chomp($cursummary);

    $oldsucc = &numsuccesses($oldsummary);
    $oldfail = &numfailures($oldsummary);
    $oldfut  = &numfutures($oldsummary);

    $cursucc = &numsuccesses($cursummary);
    $curfail = &numfailures($cursummary);
    $curfut  = &numfutures($cursummary);

    $delsucc = &delta($oldsucc, $cursucc);
    $delfail = &delta($oldfail, $curfail);
    $delfut  = &delta($oldfut, $curfut);

    $summary = "Tests run: $cursucc Successes ($delsucc), $curfail Failures ($delfail)";
} else {
    $summary = "Tests run: failed";
}

$knownumtests = 1;
if ($status == 0) {
    $oldnumtests = $oldsucc + $oldfail;
    $curnumtests = $cursucc + $curfail;
    $deltests = &delta($oldnumtests, $curnumtests);
    $numtestssummary = "$curnumtests Tests ($deltests), $curfut Futures ($delfut)";
} else {
    $numtestssummary = "unknown number of Tests";
    $knownumtests = 0;
}


#
# send mail
#
$futuremarker = "^Future";
$suppressmarker = "^Suppress";

$newfailures = "???";
if ($status == 0) {
    $newfailures = `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker" | wc -l`; chomp($newfailures);
    $newresolved = `LC_ALL=C comm -23 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker" | wc -l`; chomp($newresolved);
    $newpassingfutures = `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Success" | wc -l`; chomp($newpassingfutures);
    $passingfutures = `grep "$futuremarker" $sortedsummary | grep "\\[Success" | wc -l`; chomp($passingfutures);
    $newpassingsuppress = `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$suppressmarker" | grep "\\[Success" | wc -l`; chomp($newpassingsuppresss);
    $passingsuppresss = `grep "$suppressmarker" $sortedsummary | grep "\\[Success" | wc -l`; chomp($passingsuppresss);
    $newfailures += 0;
}

if ($newfailures == 0 && $newresolved == 0 && $newpassingfutures == 0 && $newpassingsuppress == 0) {
    print "Mailing to minimal group\n";
    $recipient = $nochangerecipient;
} else {
    print "Mailing to everyone\n";
}

$mailsubject = "$subjectid $config_name";
$mailcommand = "| $mailer -s \"$mailsubject \" $recipient";

print "Trying... $mailcommand\n";
open(MAIL, $mailcommand);

print MAIL startMailHeader($revision, $rawlog, $starttime, $endtime, $crontab, $testdirs);
print MAIL "$numtestssummary\n";
print MAIL "$summary\n";
print MAIL endMailHeader();

if ($status == 0) {
    print MAIL "--- New Errors -------------------------------\n";
    print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker"`;
    print MAIL "\n";

    print MAIL "--- Resolved Errors --------------------------\n";
    print MAIL `LC_ALL=C comm -23 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker"`;
    print MAIL "\n";
    
    print MAIL "--- New Passing Future tests------------------\n";
    print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Success"`;
    print MAIL "\n";

    print MAIL "--- Passing Future tests ---------------------\n";
    print MAIL `LC_ALL=C comm -12 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Success"`;
    print MAIL "\n";

    print MAIL "--- New Passing Suppress tests------------------\n";
    print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$suppressmarker" | grep "\\[Success"`;
    print MAIL "\n";

    print MAIL "--- Passing Suppress tests ---------------------\n";
    print MAIL `LC_ALL=C comm -12 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$suppressmarker" | grep "\\[Success"`;
    print MAIL "\n";

    print MAIL "--- Unresolved Errors ------------------------\n";
    print MAIL `LC_ALL=C comm -12 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker"`;
    print MAIL "\n";

    print MAIL "--- New Failing Future tests -----------------\n";
    print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Error"`;
    print MAIL "\n";

    print MAIL "--- Unresolved Future tests ------------------\n";
    print MAIL `LC_ALL=C comm -12 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Error"`;
    print MAIL "\n";    
}

print MAIL endMailChplenv();
close(MAIL);
    
    
#
# tuck this run's output away for comparison tomorrow
#
if ($debug == 0) {
    if ($status == 0) {
        system("cp -pv $sortedsummary $prevsummary");
    }
}
