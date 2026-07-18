CC = gcc
CFLAGS = -Wextra -Wall 
METHOD_ALLOC =linear_alloc
BINS := $(foreach m,$(METHOD_ALLOC),bin/$(m).exe)

define RULES

obj/main_$(1).o : $(1)/main.c $(1)/$(1).h
	$(CC) $(CFLAGS) -I$(1) -c $$< -o $$@

obj/$(1).o : $(1)/$(1).c $(1)/$(1).h
	$(CC) $(CFLAGS) -I$(1) -c $$< -o $$@

bin/$(1).exe : obj/$(1).o obj/main_$(1).o
	$(CC) $(CFLAGS)  $$^ -o $$@

endef

$(foreach m,$(METHOD_ALLOC),$(eval $(call RULES,$(m))))

.PHONY : all clean

all : $(BINS)

clean : 
	rm -r obj/*.o
	rm -r bin/*.exe
