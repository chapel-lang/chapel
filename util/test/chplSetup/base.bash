# We want a fresh environment not impacted by any setup the user did in their
# bashrc.  Save CHPL_HOME, clear out CHPL variables; rerun setChplEnv.
chpl_vars=$(env | grep -o "^CHPL_[^=]*")
for var in $chpl_vars; do
  unset "$var"
done

export CHPL_COMM=none  # use 'comm' feature if you wish to override this
