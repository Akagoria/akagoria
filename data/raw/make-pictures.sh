#!/bin/sh

montage icons/*.png -background none -tile 8x8 -geometry 64x64  png32:pictures/icons.png
montage sprites-128/*.png -background none -tile 8x8 -geometry 128x128  png32:tilesets/sprites-128.png
montage sprites-256/*.png -background none -tile 8x8 -geometry 256x256  png32:tilesets/sprites-256.png

montage items-s/*.png -background none -tile 16x16 -geometry 64x64  png32:../akagoria/sprites/items-s.png
