#!/usr/bin/env perl

use FindBin;
use lib "$FindBin::Bin";

use nightlysubs;
sub writeFile{

#num_args = $#ARGV + 1;
$num_args = @_;
print " ****** Number of arguments ******* length(@_) \n";
print " ****** Number of arguments ******* $num_args \n ";

# if ($num_args != 15) {
#     print "usage: nightly_email.pl \$status \$rawsummary \$sortedsummary \n";
#     print "         \$prevsummary \$mailer \$nochangerecipient \$recipient \n";
#     print "         \$subjectid \$config_name \$revision \$rawlog \$starttime \n";
#     print "         \$endtime \$crontab \$testdirs \$debug\n";
#     exit 1;
# }
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
$passed=0;
if ($status == 2) {
  $status = 0;
   print " status 2";
}

if ($status == 0) {
     print " status 0";
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
    print " status else ";
    $summary = "Tests run: failed";
    $passed = 1;
}
print " \n __________Summary __________ : \n $summary \n";

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
writeSummary ($revision,
     $starttime,
     $endtime ,
     $crontab ,
     $testdirs ,
     $numtestssummary ,
     $summary ,
     $prevsummary ,
     $sortedsummary );

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
    $passed = 1;
    print "Mailing to everyone\n";
}

# Persist the test summary to a (summary.txt) in the workspace.
# Summary.txt wull be used by Jenkins to send emails in case of a failure.
writeSummary ($revision,
     $starttime,
     $endtime ,
     $crontab ,
     $testdirs ,
     $numtestssummary ,
     $summary ,
     $prevsummary ,
     $sortedsummary );

$mailsubject = "$subjectid $config_name";
$mailcommand = "| $mailer -s \"$mailsubject \" $recipient";

if (!exists($ENV{"CHPL_TEST_NOMAIL"}) or grep {$ENV{"CHPL_TEST_NOMAIL"} =~ /^$_$/i} ('','\s*','0','f(alse)?','no?')) {
    
    $passed = 1;
    # print "Trying... $mailcommand\n";
    # open(MAIL, $mailcommand);

    # print MAIL startMailHeader($revision, $rawlog, $starttime, $endtime, $crontab, $testdirs);
    # print MAIL "$numtestssummary\n";
    # print MAIL "$summary\n";
    # print MAIL endMailHeader();

    # if ($status == 0) {
    #     print MAIL "--- New Errors -------------------------------\n";
    #     print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker"`;
    #     print MAIL "\n";

    #     print MAIL "--- Resolved Errors --------------------------\n";
    #     print MAIL `LC_ALL=C comm -23 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker"`;
    #     print MAIL "\n";

    #     print MAIL "--- New Passing Future tests------------------\n";
    #     print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Success"`;
    #     print MAIL "\n";

    #     print MAIL "--- Passing Future tests ---------------------\n";
    #     print MAIL `LC_ALL=C comm -12 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Success"`;
    #     print MAIL "\n";

    #     print MAIL "--- New Passing Suppress tests------------------\n";
    #     print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$suppressmarker" | grep "\\[Success"`;
    #     print MAIL "\n";

    #     print MAIL "--- Passing Suppress tests ---------------------\n";
    #     print MAIL `LC_ALL=C comm -12 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$suppressmarker" | grep "\\[Success"`;
    #     print MAIL "\n";

    #     print MAIL "--- Unresolved Errors ------------------------\n";
    #     print MAIL `LC_ALL=C comm -12 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep -v "$futuremarker" | grep -v "$suppressmarker"`;
    #     print MAIL "\n";

    #     print MAIL "--- New Failing Future tests -----------------\n";
    #     print MAIL `LC_ALL=C comm -13 $prevsummary $sortedsummary | grep -v "^.Summary:" | grep "$futuremarker" | grep "\\[Error"`;
    #     print MAIL "\n";
    # }

    # print MAIL endMailChplenv();
    # close(MAIL);
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

if($passed == 0)
{
    print "Got here passed == 0 \n";
  

}
else{
    print "Got here passed != 0 \n";
    
}

print "Here ..";
return $passed;

}
return(1);