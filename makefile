# ----------------------------
# Makefile Options
# ----------------------------

NAME = MEMPI
ICON = icon.png
DESCRIPTION = "Memorize pi game"
COMPRESSED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
