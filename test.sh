#!/bin/bash

# List of map paths
map_paths=(
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
          ./maps/bad/forbidden.cub
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
          ./maps/bad/textures_forbidden.cub
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

# Loop through each map path
for map_path in "${map_paths[@]}"
do
  echo -e "\n++++++++++ $map_path ++++++++++\n"
  # Run valgrind
  valgrind --leak-check=full --show-leak-kinds=all -q ./cub3D $map_path 2>&1 > output.txt
  # Print output
  cat output.txt
done

echo -e "\n++++++++++ Test Finished ++++++++++\n"

# Remove the output file
rm output.txt
