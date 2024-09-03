# Makefile for [ "$(uname -s)" = "Linux" ]

# TODO: adapt for MacOS

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
NORMINETTE_EXCLUDE_DIRS := ./norminette_exclude_dirs

# general compile flags
CFLAGS += -std=c11
# compile warning flags
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -pedantic
CFLAGS += -Wconversion
CFLAGS += -Wunreachable-code
CFLAGS += -Wshadow
CFLAGS += -Wno-overlength-strings
# performance flags
CFLAGS := -Ofast
CFLAGS += -march=native
CFLAGS += -fno-signed-zeros
CFLAGS += -funroll-loops
# Unsafe performance flags
CFLAGS += -fomit-frame-pointer
CFLAGS += -ffast-math
CFLAGS += -fno-math-errno
CFLAGS += -funsafe-math-optimizations
CFLAGS += -fassociative-math
CFLAGS += -freciprocal-math
CFLAGS += -ffinite-math-only
CFLAGS += -fno-signed-zeros
CFLAGS += -fno-trapping-math
CFLAGS += -frounding-math

# preprocessor flags
CPPFLAGS :=
CPPFLAGS += -MD
CPPFLAGS += -MP
CPPFLAGS += -I$(LIBFT_DIR)
CPPFLAGS += -I$(MINILIBX_DIR)
CPPFLAGS += -I$(SRCDIR)

# linker flags
LDFLAGS :=
LDFLAGS += -L$(LIBFT_DIR)
LDFLAGS += -L$(MINILIBX_DIR)

# linker libraries
LDLIBS :=
LDLIBS += -l$(LIBFT_LIB)
LDLIBS += -l$(MINILIBX_LIB)
LDLIBS += -lXext
LDLIBS += -lX11
LDLIBS += -lm

# DEBUG=1 make re # include debugging information in the binary
ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb3 -O0 -DDEBUG
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
SRC += mlx_helpers.c
SRC += render.c
SRC += lights.c
SRC += hoarding_disorder.c
SRC += parser.c

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

### Don't recompile, just run the program (with optional arguments)
run:
	@printf '\n'
	@# This allows $(NAME) to be run using either an absolute, relative or no path.
	@# You can pass arguments like this: make run ARGS="hello ' to this world ! ' ."
	@PATH=".$${PATH:+:$${PATH}}" && \
		$(NAME) $(ARGS)

### Don't recompile, just run the program with valgrind (and optional arguments)
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

## Compile, then run the program (with optional arguments)
rc compile-run: all
	@$(MAKE) run

## Recompile, then run the program (with optional arguments)
r rerun: re
	@$(MAKE) run

## Recompile, then run the program with valgrind (and optional arguments)
l leakcheck: re
	@$(MAKE) valrun

# TODO: benchmark between ft_printf and printf
### Don't recompile, just check for forbidden functions
fi forbidden-funcs-internal:
	@# - memset, bzero, ... can be ignored from nm (they are added by compiler)
	@# - open, close, read, write, printf, malloc, free, perror, strerror,
	@#   and exit are from the subject
	@# - ... are from the math library (-lm)
	@# - ... are from the minilibx library (-lmlx)
	@# - sqrt is from the math library
	@# - the functions below starting with underscore are added by the compiler
	@# - and a significant part of functions is also added by the mlx library
	@printf '\n'
	@$(NM) --undefined-only $(NAME)      | \
		grep -v ' bzero@'     | \
		grep -v ' memset@'    | \
		grep -v ' puts@'      | \
		grep -v ' putchar@'      | \
		grep -v ' read@'         | \
		grep -v ' free@'         | \
		grep -v ' open@'         | \
		grep -v ' exit@'         | \
		grep -v ' printf@'         | \
		grep -v ' write@'        | \
		grep -v ' close@'        | \
		grep -v ' malloc@'       | \
		grep -v ' perror@'       | \
		grep -v ' strerror@'     | \
		grep -v ' sqrt@'            | \
		grep -v ' __gmon_start__'              | \
		grep -v ' _ITM_registerTMCloneTable'   | \
		grep -v ' _ITM_deregisterTMCloneTable' | \
		grep -v ' __cxa_finalize@'             | \
		grep -v ' __errno_location@'           | \
		grep -v ' __stack_chk_fail@'           | \
		grep -v ' __libc_start_main@'          | \
		grep -v ' __printf_chk@'               | \
		grep -v ' calloc@'                       | \
		grep -v ' getenv@'                       | \
		grep -v ' gethostname@'                  | \
		grep -v ' shmat@'                        | \
		grep -v ' shmctl@'                       | \
		grep -v ' shmdt@'                        | \
		grep -v ' shmget@'                       | \
		grep -v ' strlen@'                       | \
		grep -v ' strncmp@'                      | \
		grep -v ' XChangeWindowAttributes'       | \
		grep -v ' XCopyArea'                     | \
		grep -v ' XCreateColormap'               | \
		grep -v ' XCreateGC'                     | \
		grep -v ' XCreateImage'                  | \
		grep -v ' XCreatePixmap'                 | \
		grep -v ' XCreateWindow'                 | \
		grep -v ' XFlush'                        | \
		grep -v ' XGetVisualInfo'                | \
		grep -v ' XGetWMNormalHints'             | \
		grep -v ' XInternAtom'                   | \
		grep -v ' XkbKeycodeToKeysym'            | \
		grep -v ' XMapRaised'                    | \
		grep -v ' XNextEvent'                    | \
		grep -v ' XOpenDisplay'                  | \
		grep -v ' XPending'                      | \
		grep -v ' XPutBackEvent'                 | \
		grep -v ' XPutImage'                     | \
		grep -v ' XSetClipOrigin'                | \
		grep -v ' XSetErrorHandler'              | \
		grep -v ' XSetWMNormalHints'             | \
		grep -v ' XSetWMProtocols'               | \
		grep -v ' XShmAttach'                    | \
		grep -v ' XShmCreateImage'               | \
		grep -v ' XShmCreatePixmap'              | \
		grep -v ' XShmPixmapFormat'              | \
		grep -v ' XShmPutImage'                  | \
		grep -v ' XShmQueryVersion'              | \
		grep -v ' XStoreName'                    | \
		grep -v ' XSync'                         | \
		grep -v ' XWindowEvent'                  | \
		grep -v ' XAutoRepeatOff'                | \
		grep -v ' XAutoRepeatOn'                 | \
		grep -v ' XCloseDisplay'                 | \
		grep -v ' XDestroyWindow'                | \
		grep -v ' XFreeGC'                       | \
		grep -v ' XFreePixmap'                   | \
		grep -v ' XShmDetach'                    | \
		grep ''                                  && \
		printf '\033[41;30m%s\033[m\n' "There are forbidden functions!" || \
		( \
			grep --include='*.[hc]' \
				--exclude-dir=minilibx-linux \
				--color=always \
				--with-filename \
				--line-number \
				--binary-files=without-match \
				--dereference-recursive \
				-e ' __gmon_start__' \
				-e ' _ITM_registerTMCloneTable' \
				-e ' _ITM_deregisterTMCloneTable' \
				-e ' __cxa_finalize@' \
				-e ' __errno_location@' \
				-e ' __stack_chk_fail@' \
				-e ' __libc_start_main@' \
				-e ' __printf_chk@' \
				-e 'memset' \
				-e 'bzero' \
				-e 'puts' \
				-e 'putchar' \
				-e 'calloc' \
				-e 'getenv' \
				-e 'gethostname' \
				-e 'shmat' \
				-e 'shmctl' \
				-e 'shmdt' \
				-e 'shmget' \
				-e 'strlen' \
				-e 'strncmp' \
				-e 'XChangeWindowAttributes' \
				-e 'XCopyArea' \
				-e 'XCreateColormap' \
				-e 'XCreateGC' \
				-e 'XCreateImage' \
				-e 'XCreatePixmap' \
				-e 'XCreateWindow' \
				-e 'XFlush' \
				-e 'XGetVisualInfo' \
				-e 'XGetWMNormalHints' \
				-e 'XInternAtom' \
				-e 'XkbKeycodeToKeysym' \
				-e 'XMapRaised' \
				-e 'XNextEvent' \
				-e 'XOpenDisplay' \
				-e 'XPending' \
				-e 'XPutBackEvent' \
				-e 'XPutImage' \
				-e 'XSetClipOrigin' \
				-e 'XSetErrorHandler' \
				-e 'XSetWMNormalHints' \
				-e 'XSetWMProtocols' \
				-e 'XShmAttach' \
				-e 'XShmCreateImage' \
				-e 'XShmCreatePixmap' \
				-e 'XShmPixmapFormat' \
				-e 'XShmPutImage' \
				-e 'XShmQueryVersion' \
				-e 'XStoreName' \
				-e 'XSync' \
				-e 'XWindowEvent' \
				-e 'XAutoRepeatOff' \
				-e 'XAutoRepeatOn' \
				-e 'XCloseDisplay' \
				-e 'XDestroyWindow' \
				-e 'XFreeGC' \
				-e 'XFreePixmap' \
				-e 'XShmDetach' \
				| \
			grep --invert-match '\<ft_' && \
			printf '\033[41;30m%s\033[m\n' "You've used a forbidden function!" || \
			printf '\033[42;30m%s\033[m\n' "No forbidden functions!" \
		)

## Recompile and then check for forbidden functions
f forbidden-funcs: re
	@$(MAKE) forbidden-funcs-internal

## Recompile, run the program and then check for forbidden functions
fr forbidden-funcs-run: re
	@$(MAKE) run
	@$(MAKE) forbidden-funcs-internal

## -- -- -- -- RECOMMENDED -- -- --
## Recompile, run the program with valgrind (and optional arguments), and
## then check for forbidden functions
fl forbidden-funcs-leakcheck: leakcheck
	@$(MAKE) forbidden-funcs-internal

## Recompile and check for norm
n norm: re
	$(NORMINETTE_EXCLUDE_DIRS) minilibx-linux

### Display this helpful message
h help:
	@printf '\033[31m%b\033[m\n\nTARGETs:\n' "USAGE:\n\tmake <TARGET> [ARGS=\"\"]"
	@<Makefile python3 -c 'exec('"'"'import re\n\nWIDTH = 8\nregex_self_doc = r"## [\\s\\S]*?\\n([a-z][a-zA-Z -]*):"\nmatches = list(re.finditer(regex_self_doc, open(0).read()))\nformatted_targets = []\nfor match in matches:\n    target = match.groups()[0]\n    doc_str = "\\n".join(match.group().split("\\n")[:-1]).replace("\\n", " ").replace("## ", "")\n    doc_str_words = doc_str.split()\n    doc_str_words_folded = [doc_str_words[i:i+WIDTH] for i in range(0, len(doc_str_words), WIDTH)]\n    formatted_doc_str = "\\n\\t".join([" ".join(words) for words in doc_str_words_folded])\n    formatted_targets.append(f"\\033[36m{target}\\033[m:\\n\\t{formatted_doc_str}")\nhelp_str = "\\n".join(formatted_targets)\nprint(help_str)\n'"'"')'
	@printf '\n\033[31mNOTES:\n\t%s\033[m\n' 'ARGS only makes sense when the target runs the program'

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
