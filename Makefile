TARGET = sbitx

CC      = gcc
CFLAGS  = -g $(shell pkg-config --cflags gtk+-3.0)

LDFLAGS =    \
	-lasound   \
	-lfftw3    \
	-lfftw3f   \
	-lm        \
	-lncurses  \
	-lsqlite3  \
	-lwiringPi \
	-pthread   \
	$(shell pkg-config --libs gtk+-3.0)

SRCS =          \
	$(TARGET).c   \
	fft_filter.c  \
	hamlib.c      \
	hist_disp.c   \
	ini.c         \
	log.c         \
	logbook.c     \
	macros.c      \
	modem_cw.c    \
	modem_ft8.c   \
	modems.c      \
	mongoose.c    \
	queue.c       \
	remote.c      \
	sbitx_gtk.c   \
	sbitx_sound.c \
	sbitx_utils.c \
	settings_ui.c \
	si5351v2.c    \
	si570.c       \
	telnet.c      \
	vfo.c         \
	webserver.c

OBJS    = $(SRCS:.c=.o) 
FT8_LIB = ft8_lib/libft8.a

.PHONY: all clean

all: audio data web data/sbitx.db $(TARGET)

$(TARGET): $(OBJS) $(FT8_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

log.o: log.c
	$(CC) $(CFLAGS) -DLOG_USE_COLOR -c -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

audio data web:
	mkdir $@

data/sbitx.db: | data
	cd data && sqlite3 sbitx.db < create_db.sql

clean:
	rm -f $(OBJS) $(TARGET)
