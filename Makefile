ifdef DEBUG
CFLAGS+= -g
endif
#CFLAGS  += -DEDEBUG
CFLAGS  += -Wall -Werror -DEDEBUG
#CFLAGS  += -Wall -Werror -DEDEBUG -DODEBUG
CFLAGS  += $(shell xmlrpc-c-config client --cflags) 
LDFLAGS += $(shell xmlrpc-c-config client --libs)
CFLAGS  += $(shell pkg-config --cflags libxml-2.0) -I../include/
LDFLAGS += $(shell pkg-config --libs libxml-2.0)
TARGETS=wpress
WPRESSOBJS=wpress.o utilFuncs.o clean_string.o
OBJS=$(WPRESSOBJS)

all: $(TARGETS)

wpress: $(WPRESSOBJS)
	$(CC) -o wpress $(WPRESSOBJS) $(CFLAGS) $(LDFLAGS) $(DEFINES)

clean:
	rm -f $(TARGETS) $(OBJS) 2>/dev/null
