pragma "no use ChapelStandard"

module stopInitCommDiags {
  use startInitCommDiags;

  if printInitVerboseComm {
    use CommDiagnostics;
    stopVerboseComm();
  }

  if printInitCommCounts {
    use CommDiagnostics;
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
    resetCommDiagnostics();
  }
}
