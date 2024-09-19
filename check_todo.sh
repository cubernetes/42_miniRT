#! /bin/sh

# TODO important: norminette
# TODO important: sometimes, when moving around, it stops rendering and you have to click a button to continue render. investigate
# TODO later: rotations for objects
# TODO later: compile without performance flags, since it's stricter in those cases
# TODO unimportant: turn off lighting when moving??
# TODO unimportant: create GitHub Releases
# TODO unimportant: create a small python script to generate "edge case" files
# TODO unimportant: create a small bash script to test against the "edge case" files

# TODO later: ONLY remove this file when the following command doesn't print anything anymore (except for this file)
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
