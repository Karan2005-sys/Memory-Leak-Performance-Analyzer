#!/usr/bin/env bash
set -euo pipefail
BIN="$1"
SRC="$2"
OUT="$(mktemp)"
"$BIN" --format=json --output="$OUT" "$SRC" -- -std=c++20
python3 - "$OUT" <<'PY'
import json, sys
with open(sys.argv[1], encoding='utf-8') as f:
    data = json.load(f)
assert data['metrics']['functions'] >= 1
assert data['issues'], 'expected sample issues'
assert any(issue['ruleId'] == 'MEM001' for issue in data['issues'])
PY
