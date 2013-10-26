pragma "no use ChapelStandard"

module startInitCommDiags {
  config param printInitVerboseComm = false;
  config param printInitCommCounts = false;

  if printInitVerboseComm {
    use CommDiagnostics;
    startVerboseComm();
  }

  if printInitCommCounts {
    use CommDiagnostics;
    startCommDiagnostics();
  }
}

