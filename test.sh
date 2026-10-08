#!/bin/bash

# Scènes à refuser : les dossiers eux-mêmes, puis tout maps/bad
bad_paths=(maps /maps/bad maps/bad ./maps/bad/ ./maps/bad/*)
# Scènes à accepter, vérifiées sans fenêtre avec --check
good_paths=(./maps/good/*.cub)

TIMEOUT=5
out=$(mktemp)
# git ne garde pas les droits de lecture : on retire ceux de la texture
# "interdite" le temps des tests
forbidden=textures/test/forbidden.xpm
forbidden_mode=$(stat -c %a "$forbidden" 2>/dev/null || stat -f %Lp "$forbidden")
chmod 000 "$forbidden"
trap 'chmod "$forbidden_mode" "$forbidden"; rm -f "$out"' EXIT
total=0
passed=0

# Lance "$@" en arrière-plan, le tue après $TIMEOUT s ; met le code dans rc (124 = timeout)
run_with_timeout() {
  "$@" >"$out" 2>&1 &
  local pid=$! i=0
  while kill -0 "$pid" 2>/dev/null && [ "$i" -lt $((TIMEOUT * 10)) ]; do
    sleep 0.1
    i=$((i + 1))
  done
  if kill -0 "$pid" 2>/dev/null; then
    kill "$pid" 2>/dev/null
    wait "$pid" 2>/dev/null
    rc=124
  else
    wait "$pid"
    rc=$?
  fi
}

if command -v valgrind >/dev/null 2>&1; then
  use_valgrind=true
else
  use_valgrind=false
  echo "valgrind not found: checking exit codes and error messages only"
fi

# check <attendu: bad|good> <chemin>
check() {
  local want=$1 map_path=$2 extra=()
  [ "$want" = good ] && extra=(--check)
  echo -e "\n++++++++++ $map_path ++++++++++\n"
  total=$((total + 1))
  if [ "$use_valgrind" = true ]; then
    run_with_timeout valgrind --leak-check=full --show-leak-kinds=all -q ./cub3D "${extra[@]}" "$map_path"
  else
    run_with_timeout ./cub3D "${extra[@]}" "$map_path"
  fi

  issues_found=false
  reason=""
  if [ "$rc" -eq 124 ]; then
    issues_found=true; reason="still running after ${TIMEOUT}s"
  elif [ "$rc" -ge 128 ]; then
    issues_found=true; reason="crashed (exit $rc)"
  elif [ "$want" = bad ] && { [ "$rc" -ne 1 ] || ! grep -q "Error" "$out"; }; then
    issues_found=true; reason="expected exit 1 with an Error message, got exit $rc"
  elif [ "$want" = good ] && { [ "$rc" -ne 0 ] || ! grep -q "^OK" "$out"; }; then
    issues_found=true; reason="expected the scene to load, got exit $rc"
  elif [ "$use_valgrind" = true ] && { grep -q -E "definitely lost: [1-9]" "$out" || \
       grep -q -E "indirectly lost: [1-9]" "$out" || \
       grep -q -i "still reachable" "$out" || \
       grep -q -E "Invalid (read|write)" "$out"; }; then
    issues_found=true; reason="valgrind reported a memory issue"
  fi

  if [ "$issues_found" = true ]; then
    echo -e "❌ $map_path: $reason"
  else
    echo -e "✅ $map_path"
    passed=$((passed + 1))
  fi
  cat "$out"
}

for map_path in "${bad_paths[@]}"; do check bad "$map_path"; done
for map_path in "${good_paths[@]}"; do check good "$map_path"; done

echo -e "\n++++++++++ Test Finished: $passed/$total ok ++++++++++\n"
[ "$passed" -eq "$total" ]
