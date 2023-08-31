BIN:=openvpn_ubus
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
CFLAGS:=-Wall
CPPFLAGS:=-I$(CURDIR)

LDLIBS:=-largp -lubus -lubox

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-rpath=$(RPATH) -o $@ $^ $(LDLIBS)

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) $(OBJS) $(BIN)