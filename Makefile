# Makefile for [ "$(uname -s)" = "Linux" ]

# TODO: adapt for MacOS & BSD

# config
NAME := miniRT
SRCDIR := src
OBJDIR := obj
LIBFT_DIR := libft
MINILIBX_DIR := minilibx-linux

# libs
LIBFT_FILE := libft.a
LIBFT_LIB := $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT_FILE)))
MINILIBX_FILE := libmlx_Linux.a
MINILIBX_LIB := $(patsubst lib%,%,$(patsubst %.a,%,$(MINILIBX_FILE)))

# tools
# TODO: check gcc as well
CC := cc
RM := /bin/rm -f
MKDIR := /bin/mkdir -p
NM := nm

# flags
CFLAGS := -O3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -pedantic
CFLAGS += -Wconversion
CFLAGS += -Wunreachable-code
CFLAGS += -Wshadow
CFLAGS += -Wno-overlength-strings
CFLAGS += -std=c11

CPPFLAGS :=
CPPFLAGS += -MD
CPPFLAGS += -I$(LIBFT_DIR)
CPPFLAGS += -I$(MINILIBX_DIR)
CPPFLAGS += -I$(SRCDIR)

LDFLAGS :=
LDFLAGS += -L$(LIBFT_DIR)
LDFLAGS += -L$(MINILIBX_DIR)

LDLIBS :=
LDLIBS += -lm
LDLIBS += -l$(LIBFT_LIB)
LDLIBS += -l$(MINILIBX_LIB)

# DEBUG=1 make re # include debugging information in the binary
ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb3 -O0
	LDFLAGS += -ggdb3 -O0
endif

# DEBUG=1 make re # include debugging information in the binary
ifeq ($(ASAN), 1)
	CFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif

# TSAN=1 make re # check for thread errors and data races
ifeq ($(TSAN), 1)
	CFLAGS += -fsanitize=thread
	LDFLAGS += -fsanitize=thread
endif

# sources
SRC :=
vpath %.c src
SRC += main.c

# objects
OBJ := $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))

# dependencies
DEPS := $(OBJ:.o=.d)
-include $(DEPS)

# rules
.DEFAULT_GOAL := all

## Build libft, minilibx, and the program
all: libft
	@$(MAKE) minilibx
	@$(MAKE) $(NAME)

## Build libft
libft:
	$(MAKE) -C $(LIBFT_DIR)

## Build minilibx
minilibx:
	$(MAKE) -C $(MINILIBX_DIR)

$(NAME): $(LIBFT_DIR)/$(LIBFT_FILE) $(MINILIBX_DIR)/$(MINILIBX_FILE) $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $< -c -o $@ $(CPPFLAGS) $(CFLAGS)

$(OBJDIR):
	$(MKDIR) $@

## Remove intermediate files
clean:
	$(MAKE) -C $(LIBFT_DIR) $@
	$(MAKE) -C $(MINILIBX_DIR) $@
	$(RM) $(OBJ)
	$(RM) $(DEPS)
	$(RM) -r $(OBJDIR)

## Remove intermediate files as well as the program itself
fclean: clean
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(NAME)

## Recompile the program
re: fclean
	@$(MAKE) all

## Don't recompile, just run the program (with optional arguments)
run:
	@printf '\n'
	# This allows $(NAME) to be run using either an absolute, relative or no path.
	# You can pass arguments like this: make run ARGS="hello ' to this world ! ' ."
	@PATH=".$${PATH:+:$${PATH}}" && \
		$(NAME) $(ARGS)

## Don't recompile, just run the program with valgrind (and optional arguments)
valrun:
	@printf '\n'
	@PATH=".$${PATH:+:$${PATH}}" && \
		valgrind \
			--leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--track-fds=yes \
			$(NAME) \
			$(ARGS)

## Recompile, then run the program (with optional arguments)
r rerun: re
	@$(MAKE) run

## Recompile, then run the program with valgrind (and optional arguments)
l leakcheck: re
	@$(MAKE) valrun


## Don't recompile, just check for forbidden functions
fi forbidden-funcs-internal:
	# - memset and bzero can be ignored from nm (they are added by compiler)
	# - open, close, read, write, printf, malloc, free, perror, strerror,
	#   and exit are from the subject
	# - ... are from the math library (-lm)
	# - ... are from the minilibx library (-lmlx)
	# - functions starting with __* are added by the compiler
	@printf '\n'
	@$(NM) -u $(NAME)      | \
		grep -v ' bzero@'     | \
		grep -v ' memset@'    | \
		grep -v ' read@'         | \
		grep -v ' free@'         | \
		grep -v ' open@'         | \
		grep -v ' exit@'         | \
		grep -v ' write@'        | \
		grep -v ' close@'        | \
		grep -v ' malloc@'       | \
		grep -v ' perror@'       | \
		grep -v ' strerror@'     | \
		grep -v ' __gmon_start__'      | \
		grep -v ' __errno_location@'   | \
		grep -v ' __libc_start_main@'  && \
		printf '\033[41;30m%s\033[m\n' "There are forbidden functions!" || \
		( \
			grep --include='*.[hc]' --exclude-dir=minilibx-linux -R -e 'memset' -e 'bzero' | \
			grep -v -e ft_memset -e ft_bzero && \
			printf '\033[41;30m%s\033[m\n' "You used memset/bzero (forbidden)!" || \
			printf '\033[42;30m%s\033[m\n' "No forbidden functions!" \
		)

## Recompile and then check for forbidden functions
f forbidden-funcs: re
	@$(MAKE) forbidden-funcs-internal

## -- -- -- -- RECOMMENDED -- -- --
## Recompile, run the program with valgrind (and optional arguments), and
## then check for forbidden functions
fl forbidden-funcs-leakcheck: leakcheck
	@$(MAKE) forbidden-funcs-internal

## Display this helpful message
h help:
	@printf '\033[31m%b\033[m\n\nTARGETs:\n' "USAGE:\n\tmake <TARGET> [ARGS=\"\"]"
	@<Makefile python3 -c 'exec('"'"'import re\n\nWIDTH = 8\nregex_self_doc = r"## [\\s\\S]*?\\n([a-z][a-zA-Z -]*):"\nmatches = list(re.finditer(regex_self_doc, open(0).read()))\nformatted_targets = []\nfor match in matches:\n    target = match.groups()[0]\n    doc_str = "\\n".join(match.group().split("\\n")[:-1]).replace("\\n", " ").replace("## ", "")\n    doc_str_words = doc_str.split()\n    doc_str_words_folded = [doc_str_words[i:i+WIDTH] for i in range(0, len(doc_str_words), WIDTH)]\n    formatted_doc_str = "\\n\\t".join([" ".join(words) for words in doc_str_words_folded])\n    formatted_targets.append(f"\\033[36m{target}\\033[m:\\n\\t{formatted_doc_str}")\nhelp_str = "\\n".join(formatted_targets)\nprint(help_str)\n'"'"')'
	@printf '\nNOTES:\n\t%s\n' 'ARGS only makes sense when the target runs the program'

# these targets are not files
.PHONY: all clean fclean re
.PHONY: h help
.PHONY: libft
.PHONY: run
.PHONY: r rerun
.PHONY: l leakcheck
.PHONY: f forbidden-funcs
.PHONY: fi forbidden-funcs-internal
.PHONY: fl forbidden-funcs-leakcheck

# keep intermediate (*.h, *.o, *.d, *.a) targets
.SECONDARY:

# delete failed targets
.DELETE_ON_ERROR:
