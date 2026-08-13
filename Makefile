CC = gcc
CFLAGS = -Wextra -Wall 
BIN_DIR=bin
OBJ_DIR=obj
METHOD_ALLOC =linear_alloc stack_alloc pool_alloc free_list_alloc
ALIGN = align
ALIGN_DIR=$(ALIGN)
ALIGN_OBJ=$(OBJ_DIR)/$(ALIGN).o
BINS := $(foreach m,$(METHOD_ALLOC),$(BIN_DIR)/$(m).exe)

$(ALIGN_OBJ) : $(ALIGN_DIR)/$(ALIGN).c $(ALIGN_DIR)/$(ALIGN).h 
	$(CC) $(CFLAGS) -I $(ALIGN_DIR) -c $< -o $@

define RULES
$(1) : $(BIN_DIR)/$(1).exe
	$(BIN_DIR)/$(1).exe

$(OBJ_DIR)/main_$(1).o : $(1)/main.c $(1)/$(1).h
	$(CC) $(CFLAGS) -I$(1) -c $$< -o $$@

$(OBJ_DIR)/$(1).o : $(1)/$(1).c $(1)/$(1).h
	$(CC) $(CFLAGS) -I$(1) -I$(ALIGN_DIR) -c $$< -o $$@

$(BIN_DIR)/$(1).exe : obj/$(1).o obj/main_$(1).o $(ALIGN_OBJ)
	$(CC) $(CFLAGS)  $$^ -o $$@

endef

$(foreach m,$(METHOD_ALLOC),$(eval $(call RULES,$(m))))

.PHONY : all clean $(METHOD_ALLOC)

all : $(BINS)

clean : 
	rm -r obj/*.o
	rm -r bin/*.exe
