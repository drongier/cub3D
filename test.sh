#!/bin/bash

# List of map paths
map_paths=(
        maps
	/maps/bad
	maps/bad
	./maps/bad/
          ./maps/bad/color_invalid_rgb.cub
          ./maps/bad/color_missing_ceiling_rgb.cub
          ./maps/bad/color_missing.cub
          ./maps/bad/color_missing_floor_rgb.cub
          ./maps/bad/color_none.cub
          ./maps/bad/empty.cub
          ./maps/bad/error_map_borders.cub
          ./maps/bad/error_map_chars.cub
          ./maps/bad/error_map_chars_on_line.cub
          ./maps/bad/error_map_colors_oor.cub
          ./maps/bad/error_map_empty.cub
          ./maps/bad/error_map_ending.txt
          ./maps/bad/error_map_missing_color.cub
          ./maps/bad/error_map_missing_map.cub
          ./maps/bad/error_map_missing_text.cub
          ./maps/bad/error_map_multiple_color.cub
          ./maps/bad/error_map_multiple_player_pos.cub
          ./maps/bad/error_map_multiple_textures.cub
          ./maps/bad/error_map_order.cub
          ./maps/bad/error_map_player_in_wall.cub
          ./maps/bad/error_map_player_outside_wall.cub
          ./maps/bad/error_map_spaces_on_line.cub
          ./maps/bad/error_map_wrong_identifier.cub
          ./maps/bad/error_map_wrong_texture.cub
          ./maps/bad/file_letter_end.cub
          ./maps/bad/filetype_missing
          ./maps/bad/filetype_wrong.buc
          #./maps/bad/forbidden.cub
          ./maps/bad/map_first.cub
          ./maps/bad/map_middle.cub
          ./maps/bad/map_missing.cub
          ./maps/bad/map_only.cub
          ./maps/bad/map_too_small.cub
          ./maps/bad/player_multiple.cub
          ./maps/bad/player_none.cub
          ./maps/bad/player_on_edge.cub
          ./maps/bad/textures_dir.cub
          ./maps/bad/textures_duplicates.cub
          #./maps/bad/textures_forbidden.cub
          ./maps/bad/textures_invalid.cub
          ./maps/bad/textures_missing.cub
          ./maps/bad/textures_none.cub
          ./maps/bad/textures_not_xpm.cub
          ./maps/bad/wall_hole_east.cub
          ./maps/bad/wall_hole_north.cub
          ./maps/bad/wall_hole_south.cub
          ./maps/bad/wall_hole_west.cub
          ./maps/bad/wall_none.cub
            # ./maps/good/cheese_maze.cub
            # ./maps/good/creepy.cub
            # ./maps/good/dungeon.cub
            # ./maps/good/library.cub
            # ./maps/good/map.cub
            # ./maps/good/matrix.cub
            # ./maps/good/sad_face.cub
            # ./maps/good/small.cub
            # ./maps/good/square_map.cub
            # ./maps/good/subject_map.cub
            # ./maps/good/test_map.cub
            # ./maps/good/test_map_hole.cub
            # ./maps/good/test_pos_bottom.cub
            # ./maps/good/test_pos_left.cub
            # ./maps/good/test_pos_right.cub
            # ./maps/good/test_pos_top.cub
            # ./maps/good/test_textures.cub
            # ./maps/good/test_whitespace.cub
            # ./maps/good/valid_map_1.cub
            # ./maps/good/valid_map_2.cub
            # ./maps/good/valid_map_3.cub
            # ./maps/good/valid_map_4.cub
            # ./maps/good/works.cub
)

TIMEOUT=5
out=$(mktemp)
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

for map_path in "${map_paths[@]}"
do
  echo -e "\n++++++++++ $map_path ++++++++++\n"
  total=$((total + 1))
  if [ "$use_valgrind" = true ]; then
    run_with_timeout valgrind --leak-check=full --show-leak-kinds=all -q ./cub3D "$map_path"
  else
    run_with_timeout ./cub3D "$map_path"
  fi

  issues_found=false
  reason=""
  if [ "$rc" -eq 124 ]; then
    issues_found=true; reason="still running after ${TIMEOUT}s (map accepted?)"
  elif [ "$rc" -ge 128 ]; then
    issues_found=true; reason="crashed (exit $rc)"
  elif [ "$rc" -ne 1 ] || ! grep -q "Error" "$out"; then
    issues_found=true; reason="expected exit 1 with an Error message, got exit $rc"
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
done

rm -f "$out"
echo -e "\n++++++++++ Test Finished: $passed/$total ok ++++++++++\n"
[ "$passed" -eq "$total" ]
