# Uncomment following to enable debug mode:
# DEBUG    = yes

CC       = g++
EXAMPLES = plain_to_html filter_html

CFLAGS   = -Wall
ifeq '$(DEBUG)' 'yes'
CFLAGS  += -g -O0
else
CFLAGS  += -O3
endif

INCLUDE  = -I ../src
LFLAGS   = -L.. -ltexo


default: $(EXAMPLES)

%: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) $< $(LFLAGS) -o $@

clean:
	rm -f $(EXAMPLES)
