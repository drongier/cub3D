#!/bin/bash

# List of map paths
map_paths=(
    "maps/error_map_borders.cub"
    "maps/error_map_chars_on_line.cub"
    "maps/error_map_chars.cub"
    "maps/error_map_colors_oor.cub"
    "maps/error_map_empty.cub"
    "maps/error_map_ending.txt"
    "maps/error_map_missing_color.cub"
    "maps/error_map_multiple_color.cub"
    "maps/error_map_multiple_textures.cub"
    "maps/error_map_missing_map.cub"
    "maps/error_map_missing_text.cub"
    "maps/error_map_multiple_player_pos.cub"
    "maps/error_map_order.cub"
    "maps/error_map_player_in_wall.cub"
    "maps/error_map_player_outside_wall.cub"
    "maps/error_map_spaces_on_line.cub"
    "maps/error_map_wrong_identifier.cub"
    "maps/error_map_wrong_texture.cub"
    "maps/valid_map_1.cub"
    "maps/valid_map_2.cub"
    "maps/valid_map_3.cub"
    "maps/valid_map_4.cub"
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