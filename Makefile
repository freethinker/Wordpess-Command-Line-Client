ifdef DEBUG
CFLAGS+= -g
endif
CFLAGS  += -Wall -Werror -DEDEBUG
#CFLAGS  += -Wall -Werror -DEDEBUG -DODEBUG
CFLAGS  += $(shell xmlrpc-c-config client --cflags) 
LDFLAGS += $(shell xmlrpc-c-config client --libs)
CFLAGS  += $(shell pkg-config --cflags libxml-2.0) -I../include/
LDFLAGS += $(shell pkg-config --libs libxml-2.0)
TARGETS=wppost
WPPOSTOBJS=wppost.o utilFuncs.o
OBJS=$(WPPOSTOBJS)

all: $(TARGETS)

wppost: $(WPPOSTOBJS)
	$(CC) -o wppost $(WPPOSTOBJS) $(CFLAGS) $(LDFLAGS) $(DEFINES)

clean:
	rm -f $(TARGETS) $(OBJS) 2>/dev/null
