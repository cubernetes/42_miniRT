#! /bin/sh

# TODO: create GitHub Releases
# TODO: benchmark float (e.g. sqrtf) against double (e.g. sqrt)
# TODO: fix that mlx valgrind error regarding writing to uninitialized bytes

# TODO: add precision functions (round, etc.)
# TODO: add diffuse lighting
# TODO: understand and fix lighting artefacts
# TODO: rotations for objects and camera
# TODO: create a small python script to generate "edge case" files
# TODO: create a small bash script to test against the "edge case" files
# TODO: norminette
# TODO: reduce resolution when doing anything, auto-hd-render after 1s or so

# TODO: ONLY remove this file when the following command doesn't print anything anymore (except for this file)
grep --color=always \
	--exclude-dir=.git/ \
	--exclude-dir=data_constructors \
	--exclude-dir=deque \
	--exclude-dir=gc \
	--exclude-dir=hashtable \
	--exclude-dir=io \
	--exclude-dir=list \
	--exclude-dir=math \
	--exclude-dir=memory \
	--exclude-dir=misc \
	--exclude-dir=string \
	--exclude=ft_replace.h \
	--exclude=libft.h \
	-inR -- '[T]ODO'
