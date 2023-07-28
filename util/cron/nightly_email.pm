#!/usr/bin/env perl

use FindBin;
use lib "$FindBin::Bin";

use nightlysubs;

sub writeFile{
$num_args = @_;
print " \n  Number of arguments  $num_args \n ";

if ($num_args != 16) {
    print "usage: nightly_email.pl \$status \$rawsummary \$sortedsummary \n";
    print "         \$prevsummary \$mailer \$nochangerecipient \$recipient \n";
    print "         \$subjectid \$config_name \$revision \$rawlog \$starttime \n";
    print "         \$endtime \$crontab \$testdirs \$debug\n";
    exit 1;
}
my ($status, $rawsummary, $sortedsummary, ,$prevsummary, $mailer, $nochangerecipient, $recipient, $subjectid, $config_name, $revision, $rawlog, $starttime, $endtime, $crontab, $testdirs, $debug)=@_;

$status = $_[0];
$rawsummary = $_[1];
$sortedsummary = $_[2];
$prevsummary = $_[3];
$mailer = $_[4];
$nochangerecipient = $_[5];
$recipient = $_[6];
$subjectid = $_[7];
$config_name = $_[8];
$revision = $_[9];
$rawlog = $_[10];
$starttime = $_[11];
$endtime = $_[12];
$crontab = $_[13];
$testdirs = $_[14];
$debug = $_[15];


# Ensure the "previous" summary exists, e.g. if this is the first run of the
# configuration they won't.
ensureSummaryExists($prevsummary);


#
# sort output
#

# status 2 means tests passed and there were some failures.
# that shouldn't change the format of the email, so we collapse
# the cases here.
$email=1;
if ($status == 2) {
  $status = 0;
  
}

if ($status == 0) {
     print " \n status 0 \n";
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
    $summary = "Tests run: failed new failures";
    $passed = 1;
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
# writeEmail ($revision,
#      $starttime,
#      $endtime ,
#      $crontab ,
#      $testdirs ,
#      $numtestssummary ,
#      $summary ,
#      $prevsummary ,
#      $sortedsummary );

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
    $email=0;
    print "Mailing to minimal group\n";
    $recipient = $nochangerecipient;

} else {
    
    print "Mailing to everyone\n";
}

# Persist the test summary to a (email.txt) in the jenkins workspace.
# email.txt will be used by Jenkins to send emails.


if (!exists($ENV{"CHPL_TEST_NOMAIL"}) or grep {$ENV{"CHPL_TEST_NOMAIL"} =~ /^$_$/i} ('','\s*','0','f(alse)?','no?')) {
    
    print "\n Email:$email \n status:$status \n";
    if ($email == 1){
    writeEmail ($revision,
     $starttime,
     $endtime ,
     $crontab ,
     $testdirs ,
     $numtestssummary ,
     $summary ,
     $prevsummary ,
     $sortedsummary );
    }
} else {
    print "CHPL_TEST_NOMAIL: No $mailcommand\n";
}

#
# tuck this run's output away for comparison tomorrow
#
if ($debug == 0) {
    if ($status == 0) {
        system("cp -pv $sortedsummary $prevsummary");
    }
 }

return(0);

}

return(1);