# config
NAME := libft.a
SRCDIR := .
OBJDIR := obj

# tools
CC := cc
AR := ar rcs
RM := /bin/rm -f
MKDIR := /bin/mkdir -p

# general compile flags
CFLAGS :=
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
# performance flags (don't do much tbh)
CFLAGS += -Ofast
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
CPPFLAGS += -I$(SRCDIR)

ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb3 -O0
endif

ifeq ($(ASAN), 1)
	CFLAGS += -fsanitize=address
endif

ifeq ($(TSAN), 1)
	CFLAGS += -fsanitize=thread
endif

# sources
SRC :=
vpath %.c misc
SRC += ft_arrlen.c
SRC += error_handling.c
SRC += cmt.c
SRC += ft_getopt.c
SRC += ft_sleep_linux.c
SRC += ft_uptime_linux.c

vpath %.c string
SRC += ft_atoi.c
SRC += ft_atoi_status.c
SRC += ft_atol_status.c
SRC += ft_atof.c
SRC += ft_strtof.c
SRC += ft_isalnum.c
SRC += ft_isalpha.c
SRC += ft_isascii.c
SRC += ft_isdigit.c
SRC += ft_isprint.c
SRC += ft_strchr.c
SRC += ft_strdup.c
SRC += ft_nullable_strdup.c
SRC += ft_strndup.c
SRC += ft_strlcat.c
SRC += ft_strlcpy.c
SRC += ft_strlen.c
SRC += ft_strncmp.c
SRC += ft_strcmp.c
SRC += ft_nullable_strcmp.c
SRC += ft_streq.c
SRC += ft_strnstr.c
SRC += ft_strrchr.c
SRC += ft_tolower.c
SRC += ft_toupper.c
SRC += ft_itoa.c
SRC += ft_itoa_static.c
SRC += ft_strtrim.c
SRC += ft_substr.c
SRC += ft_strjoin.c
SRC += ft_split.c
SRC += ft_striteri.c
SRC += ft_strmapi.c
SRC += ft_char_in_charset.c
SRC += ft_isspace.c
SRC += uniquize.c
SRC += ft_count_all.c
SRC += ft_replace.c
SRC += ft_replace_all.c

vpath %.c memory
SRC += ft_bzero.c
SRC += ft_calloc.c
SRC += ft_memchr.c
SRC += ft_memcmp.c
SRC += ft_memcpy.c
SRC += ft_memdup.c
SRC += ft_memmove.c
SRC += ft_memset.c
SRC += strip_nul.c

vpath %.c gc
SRC += set_allocator.c
SRC += get_allocator.c
SRC += gc_set_context.c
SRC += gc_malloc.c
SRC += gc_start_context.c
SRC += gc_end_context.c
SRC += gc_get_context.c
SRC += gc_get_context_name.c
SRC += gc_free.c
SRC += gc_free_all.c
SRC += gc_ctx_manager.c
SRC += gc_add_str.c
SRC += gc_add.c
SRC += ft_malloc.c
SRC += ft_free.c
SRC += dont_free.c

vpath %.c io
SRC += get_next_line.c
SRC += get_next_fat_line.c
SRC += ft_putchar_fd.c
SRC += ft_putendl_fd.c
SRC += ft_putnbr_fd.c
SRC += ft_putstr_fd.c
SRC += ft_putendl.c
SRC += ft_putstr.c
SRC += ft_putstr_n.c
SRC += ft_putstr_n_clr.c
SRC += ft_putnbr.c
SRC += ft_putchar.c
SRC += ft_putfmt.c
SRC += ft_putfmt_fd.c
SRC += ft_puthex_fd.c
SRC += ft_putptr_fd.c
SRC += ft_puthex.c
SRC += ft_putptr.c
SRC += ft_printf.c
SRC += ft_dprintf.c
SRC += ft_print_memory.c

vpath %.c math
SRC += ft_max.c
SRC += ft_abs.c
SRC += ft_min.c
SRC += cmp_int_desc.c
SRC += cmp_int_asc.c
SRC += ft_atof.c
SRC += ft_rand.c

vpath %.c list
SRC += lswap.c
SRC += ljoin.c
SRC += lsplit.c
SRC += lsplit_n.c
SRC += lsort.c
SRC += lrotate.c
SRC += lpush_left.c
SRC += lpush.c
SRC += lprint_rev.c
SRC += lprint.c
SRC += lpop_left.c
SRC += ltop_left.c
SRC += lpop.c
SRC += ltop.c
SRC += lnext.c
SRC += lnew.c
SRC += llast.c
SRC += liter_rev.c
SRC += liter.c
SRC += lforward.c
SRC += lextend_left.c
SRC += lextend.c
SRC += lequal.c
SRC += ldestroy.c
SRC += lcopy.c
SRC += lbackward.c
SRC += ltoarr.c

vpath %.c data_constructors
SRC += as_pointer_types.c
SRC += as_pointer_types_2.c
SRC += as_numeric_types.c
SRC += as_numeric_types_2.c
SRC += as_enum_types.c
SRC += as_data.c

vpath %.c hashtable
SRC += ht_unset.c
SRC += ht_set.c
SRC += ht_get.c
SRC += ht_print.c
SRC += ht_destroy.c
SRC += ht_to_list.c
SRC += ht_hash.c

vpath %.c geometry/vec3
SRC += vec3.c
SRC += vec3_operations1.c
SRC += vec3_operations2.c

vpath %.c geometry/ray
SRC += ray.c

vpath %.c geometry/sphere
SRC += sphere.c

vpath %.c geometry/plane
SRC += plane.c
SRC += plane_rotate.c

vpath %.c geometry/cylinder
SRC += cylinder_rotate.c
SRC += cylinder.c
SRC += cylinder_utils.c

vpath %.c geometry/quaternion
SRC += quat_add.c
SRC += quat_copy.c
SRC += quat_helper.c
SRC += quat_mult.c

vpath %.c colors
SRC += colors.c
SRC += colors_getters.c

# objects
OBJ := $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))

# dependencies
DEPS := $(OBJ:.o=.d)
-include $(DEPS)

# rules
.DEFAULT_GOAL := all

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $@ $(OBJ)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $< -c -o $@ $(CFLAGS) $(CPPFLAGS)

$(OBJDIR):
	$(MKDIR) $@

clean:
	$(RM) $(OBJ)
	$(RM) $(DEPS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: re fclean clean all

.SECONDARY:

.DELETE_ON_ERROR:
