# Uncomment following to enable debug mode:
DEBUG    = yes

CC        = g++
EXAMPLES  = plain_to_html

CPPFLAGS  = -Wall -ansi
ifeq '$(DEBUG)' 'yes'
CPPFLAGS += -g -O0
else
CPPFLAGS += -O3
endif

INCLUDE   = -I../src -I$(SCRIPTDIR)
LFLAGS    = -L.. -ltexo -L$(SCRIPTDIR) -lscriptpp

TEXO      = ../libtexo.a
SCRIPTDIR = ../../lib/scriptpp
SCRIPTPP  = $(SCRIPTDIR)/libscriptpp.a


default: $(EXAMPLES)

$(TEXO):
	cd ..; $(MAKE)

$(SCRIPTPP):
	cd $(SCRIPTDIR); $(MAKE)

%: %.cpp $(SCRIPTPP) $(TEXO)
	$(CC) $(CPPFLAGS) $(INCLUDE) $< $(LFLAGS) -o $@

clean:
	rm -f $(EXAMPLES)
