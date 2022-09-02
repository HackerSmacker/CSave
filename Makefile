# UNIX makefile

# Your C compiler.
CC = cc
LD = cc

# Linker library paths.
LDFLAGS = -lprotobuf-c -L/usr/local/lib -L/opt/homebrew/lib
CFLAGS = -I. -I/usr/local/include -I/opt/homebrew/include -std=c89 -g

# Input protobuf def files
PROTOS = OakCommon.proto OakSave.proto OakProfile.proto

# Resultant objects (both library and program)
OBJS = OakProfile.pb-c.o OakSave.pb-c.o OakShared.pb-c.o SaveUnpack.o FileTranslator.o SaveToProto.o ItemCode.o SaveGenerate.o ItemLookupTest.o ProtoToSave.o CSaveGUI.o ProfileUnpack.o ProfileGenerate.o ProtoToProfile.o ProfileToProto.o SaveConvert.o CSaveMenu.o ReadSaveHeader.o

# Results of the protobuf generation process
CPROTOS = OakProfile.pb-c.c OakSave.pb-c.c OakShared.pb-c.c

# All programs
PROGS = SaveUnpack SaveToProto SaveGenerate ItemLookupTest ProtoToSave CSaveGUI ProfileUnpack ProfileGenerate ProtoToProfile ProfileToProto SaveConvert CSaveMenu ReadSaveHeader

# Embeddable library
LIBRARIES = libBL3Proto.a

# Manuals
MANUALS_PS = manual.ps
MANUALS_PDF = manual.pdf
MANUALS_TXT = manual.txt

# Install prefix (they'll go in /usr/local/bin by default)
PREFIX = /usr/local

all: $(PROGS)

.SUFFIXES: .ps .ms .pdf .txt

.ms.txt:
	troff -Tascii -ms $< > $@

.ms.ps:
	groff -Tps -ms $< > $@

.ps.pdf:
	ps2pdf $< $@

.SUFFIXES: .c .h .o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

# Workaround for protoc-c's weird (bad) output name format.
# You can't use a suffix rule here, sadly.
$(CPROTOS): *.proto
	protoc-c --c_out=. OakProfile.proto
	protoc-c --c_out=. OakSave.proto
	protoc-c --c_out=. OakShared.proto

# Rule to produce the embeddable library.
$(LIBRARIES): $(CPROTOS) $(OBJS)
	ar cr libBL3Proto.a OakProfile.pb-c.o OakSave.pb-c.o OakShared.pb-c.o FileTranslator.o ItemCode.o

$(PROGS): $(LIBRARIES)
	$(LD) -o SaveUnpack SaveUnpack.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o SaveToProto SaveToProto.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o ProtoToSave ProtoToSave.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o SaveGenerate SaveGenerate.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o ItemLookupTest ItemLookupTest.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o ProfileUnpack ProfileUnpack.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o ProfileGenerate ProfileGenerate.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o ProfileToProto ProfileToProto.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o ProtoToProfile ProtoToProfile.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o SaveConvert SaveConvert.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o ReadSaveHeader ReadSaveHeader.o libBL3Proto.a $(LDFLAGS)
	$(LD) -o CSaveGUI CSaveGUI.o -lncurses
	$(LD) -o CSaveMenu CSaveMenu.o

manual: $(MANUALS_PS) $(MANUALS_PDF)

install: $(PROGS)
	install SaveUnpack $(PREFIX)/bin
	install SaveToProto $(PREFIX)/bin
	install SaveGenerate $(PREFIX)/bin
	install ProtoToSave $(PREFIX)/bin
	install ProfileUnpack $(PREFIX)/bin
	install ProfileGenerate $(PREFIX)/bin
	install CSaveGUI $(PREFIX)/bin
	install CSaveMenu $(PREFIX)/bin
	install ProtoToProfile $(PREFIX)/bin
	install ProfileToProto $(PREFIX)/bin
	install SaveConvert $(PREFIX)/bin
	install ReadSaveHeader $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/bin/SaveUnpack
	rm $(PREFIX)/bin/SaveToProto
	rm $(PREFIX)/bin/ProtoToSave
	rm $(PREFIX)/bin/SaveGenerate
	rm $(PREFIX)/bin/ProfileGenerate
	rm $(PREFIX)/bin/ProfileUnpack
	rm $(PREFIX)/bin/CSaveGUI
	rm $(PREFIX)/bin/CSaveMenu
	rm $(PREFIX)/bin/ProtoToProfile
	rm $(PREFIX)/bin/ProfileToProto
	rm $(PREFIX)/bin/SaveConvert
	rm $(PREFIX)/bin/ReadSaveHeader


.PHONY: clean

clean:
	rm -f *.o Oak*.pb-c.* *.a $(MANUALS_PS) $(MANUALS_PDF) $(MANUALS_TXT) $(PROGS)
