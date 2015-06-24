#
# nightly subroutines
#

use File::Basename;
use Cwd;
use Cwd 'abs_path';
use Sys::Hostname;

$cwd = abs_path(dirname(__FILE__));
$chplhomedir = abs_path("$cwd/../..");


sub mysystem {
    $command = $_[0];
    $errorname = $_[1];
    $fatal = $_[2];
    $mailmsg = $_[3];
    $showcommand = $_[4];

    if ($showcommand) { print "Executing $command\n"; }
    my $status = system($command);
    if ($status != 0) {
        $endtime = localtime;
	$somethingfailed = 1;
        if($status != -1) {$status = $status / 256; }
	print "Error $errorname: $status\n";

	if ($mailmsg != 0) {
            $mailsubject = "$subjectid $config_name Failure";
            $mailcommand = "| $mailer -s \"$mailsubject \" $recipient";

            print "Trying to mail message... using $mailcommand\n";
	    open(MAIL, $mailcommand);
            print MAIL startMailHeader($revision, $rawlog, $starttime, $endtime, $crontab, "");
	    print MAIL "ERROR $errorname: $status\n";
	    print MAIL "(workspace left at $tmpdir)\n";
            print MAIL endMailHeader();
            print MAIL endMailChplenv();
	    close(MAIL);
	}

        if ($fatal != 0) {
            exit 1;
        }
    }
    $status;
}

sub numsuccesses {
  my $mystr = shift;
  my $ret = 0;
  if( $mystr =~ /#Successes = (\d+)/ ) {
    $ret = $1;
  }
  return $ret;
}

sub numfailures {
  my $mystr = shift;
  my $ret = 0;
  if( $mystr =~ /#Failures = (\d+)/ ) {
    $ret = $1;
  }
  return $ret;
}

sub numfutures {
  my $mystr = shift;
  my $ret = 0;
  if( $mystr =~ /#Futures = (\d+)/ ) {
    $ret = $1;
  }
  return $ret;
}

sub delta {
  $delta = $_[1] - $_[0];
  if ($delta >= 0) {
      $delta = "+$delta";
  }
  $delta;
}

sub ensureSummaryExists {
    $summary = $_[0];
    if (! -r $summary) {
        print "Creating $summary\n";
        `echo "[Summary: #Successes = 0 | #Failures = 0 | #Futures = 0]" > $summary`
    }
}

sub startMailHeader {
    my $revision = $_[0];
    my $logfile = $_[1];
    my $started = $_[2];
    my $ended = $_[3];
    my $crontab = $_[4];
    my $tests_run = $_[5];
    chomp($tests_run);

    my $mystr =
        "=== Summary ===================================================\n" .
        "Hostname: " . hostname . "\n" .
        $revision . "\n" .
        "Logfile:  " . $logfile . "\n" .
        "Started:  " . $started . "\n" .
        "Ended:    " . $ended . "\n" .
        "Tests run: " . $tests_run . "\n\n";

    if ($debug == 0) {
        $mystr .= $crontab . "\n\n";
    }

    $mystr;
}

sub endMailHeader {
    my $mystr =
        "===============================================================\n\n";

    $mystr;
}

sub endMailChplenv {
    my $ch = $chplhomedir;
    if (exists($ENV{"CHPL_HOME"})) {
        $ch = $ENV{"CHPL_HOME"};
    }
    my $chplenv = `$ch/util/printchplenv --debug`;

    my $mystr = 
        "===============================================================\n" .
        "Chapel Environment:\n" .
        $chplenv . "\n";

    $mystr;
}

return(1);
