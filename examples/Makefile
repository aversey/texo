# Uncomment following to enable debug mode:
# DEBUG    = yes

CC        = g++
EXAMPLES  = plain lines markdown

CPPFLAGS  = -Wall -ansi -pedantic
ifeq '$(DEBUG)' 'yes'
CPPFLAGS += -g -O0
else
CPPFLAGS += -O3
endif

INCLUDE   = -I../src
LFLAGS    = -L.. -ltexo

TEXO      = ../libtexo.a


default: $(EXAMPLES)

$(TEXO):
	cd ..; $(MAKE)

%: %.cpp $(TEXO)
	$(CC) $(CPPFLAGS) $(INCLUDE) $< $(LFLAGS) -o $@

clean:
	rm -f $(EXAMPLES)
