#!/bin/bash

# List of map paths
map_paths=(
    "maps/bad/error_map_borders.cub"
    "maps/bad/error_map_chars_on_line.cub"
    "maps/bad/error_map_chars.cub"
    "maps/bad/error_map_colors_oor.cub"
    "maps/bad/error_map_empty.cub"
    "maps/bad/error_map_ending.txt"
    "maps/bad/error_map_missing_color.cub"
    "maps/bad/error_map_multiple_color.cub"
    "maps/bad/error_map_multiple_textures.cub"
    "maps/bad/error_map_missing_map.cub"
    "maps/bad/error_map_missing_text.cub"
    "maps/bad/error_map_multiple_player_pos.cub"
    "maps/bad/error_map_order.cub"
    "maps/bad/error_map_player_in_wall.cub"
    "maps/bad/error_map_player_outside_wall.cub"
    "maps/bad/error_map_spaces_on_line.cub"
    "maps/bad/error_map_wrong_identifier.cub"
    "maps/bad/error_map_wrong_texture.cub"
    "maps/good/valid_map_1.cub"
    "maps/good/valid_map_2.cub"
    "maps/good/valid_map_3.cub"
    "maps/good/valid_map_4.cub"
)

# Loop through each map path
for map_path in "${map_paths[@]}"
do
  echo -e "\n++++++++++ $map_path ++++++++++\n"
  # Run valgrind
  valgrind --leak-check=full --show-leak-kinds=all -q ./cub3D $map_path > output.txt 2>&1
  # Print output
  cat output.txt
done

echo -e "\n++++++++++ Test Finished ++++++++++\n"

# Remove the output file
rm output.txt
