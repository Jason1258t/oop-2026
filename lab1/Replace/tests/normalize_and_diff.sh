#!/bin/sh
set -euo pipefail
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <actual> <expected>" >&2
  exit 2
fi
actual="$1"
expected="$2"
outdir="$(dirname "$actual")"
nout="$outdir/normalized_output.txt"
nept="${outdir}/normalized_expected.txt"
python3 - <<PY
from pathlib import Path
Path('$nout').write_bytes(Path('$actual').read_bytes().rstrip(b'\n'))
Path('$nept').write_bytes(Path('$expected').read_bytes().rstrip(b'\n'))
PY

diff -u "$nept" "$nout"
