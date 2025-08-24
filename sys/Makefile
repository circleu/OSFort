CURDIR = sys
CFLAGS = -ffreestanding -nostdlib -nolibc -fno-stack-protector -m64 -masm=att -mno-sse -mno-mmx -mno-80387 -Iinc
SOURCE = $(wildcard $(CURDIR)/*.c)
OBJECT = $(subst $(CURDIR)/,tmp/$(CURDIR)/,$(patsubst %.c,%.o,$(SOURCE)))


all : $(OBJECT)

tmp/$(CURDIR)/%.o : $(CURDIR)/%.c
	gcc $(CFLAGS) -c $< -o $@