CC = gcc
LD = gcc
LDFLAGS = -lprotobuf-c -L/usr/local/lib -L/usr/lib
CFLAGS = -I. -O0
DEPS = include/*.h
PROTOS = *.proto
OBJS = OakProfile.pb-c.o OakSave.pb-c.o OakShared.pb-c.o SaveUnpack.o FileTranslator.o SaveToProto.o ItemCode.o SaveGenerate.o ItemLookupTest.o ProtoToSave.o CSaveGUI.o ProfileUnpack.o ProfileGenerate.o ProtoToProfile.o ProfileToProto.o SaveConvert.o CSaveMenu.o
CPROTOS = OakProfile.pb-c.c OakSave.pb-c.c OakShared.pb-c.c
PROGS = SaveUnpack SaveToProto SaveGenerate ItemLookupTest ProtoToSave CSaveGUI ProfileUnpack ProfileGenerate ProtoToProfile ProfileToProto SaveConvert CSaveMenu
LIBRARIES = libBL3Proto.a
MANUALS_PS = manual.ps
MANUALS_PDF = manual.pdf
MANUALS_TEXT = manual.txt
PREFIX = /usr/local


all: $(PROGS)

%.txt: %.ms
	@echo " TROFF   " $< " TEXT"
	# @groff -Tascii -ms -Z $< | grotty -c | sed 's/[\x01-\x1F\x7F]//g' | sed 's/\([A-Za-z.,()]\)\1\+/\1/g' > $@
	@groff -Tascii -ms -Z $< | grotty -c | sed 's/[\x01-\x1F\x7F]//g' > $@


%.ps: %.ms
	@echo " TROFF   " $<
	@groff -Tps -ms $< > $@

%.pdf: %.ps
	@echo " PS2PDF   " $<
	@ps2pdf $< $@

%.o: %.c
	@echo " CC   " $<
	@$(CC) $(CFLAGS) -c -o $@ $<

%.pb-c.c: %.proto
	@echo " PROTOC   " $<
	@protoc-c --c_out=. $<

$(LIBRARIES): $(CPROTOS) $(OBJS)
	@echo " AR    libBL3Proto"
	@ar cr libBL3Proto.a OakProfile.pb-c.o OakSave.pb-c.o OakShared.pb-c.o SaveToProto.o ItemCode.o FileTranslator.o

$(PROGS): $(LIBRARIES)
	@echo " LD    SaveUnpack"
	@$(LD) -o SaveUnpack SaveUnpack.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    SaveToProto"
	@$(LD) -o SaveToProto SaveToProto.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    ProtoToSave"
	@$(LD) -o ProtoToSave ProtoToSave.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    SaveGenerate"
	@$(LD) -o SaveGenerate SaveGenerate.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    ItemLookupTest"
	@$(LD) -o ItemLookupTest ItemLookupTest.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    ProfileUnpack"
	@$(LD) -o ProfileUnpack ProfileUnpack.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    ProfileGenerate"
	@$(LD) -o ProfileGenerate ProfileGenerate.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    ProfileToProto"
	@$(LD) -o ProfileToProto ProfileToProto.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    ProtoToProfile"
	@$(LD) -o ProtoToProfile ProtoToProfile.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    SaveConvert"
	@$(LD) -o SaveConvert SaveConvert.o libBL3Proto.a $(LDFLAGS)
	@echo " LD    CSaveGUI"
	@$(LD) -o CSaveGUI CSaveGUI.o -lncurses
	@echo " LD    CSaveMenu"
	@$(LD) -o CSaveMenu CSaveMenu.o

manual: $(MANUALS_PS) $(MANUALS_PDF) $(MANUALS_TEXT)

install: $(PROGS) manual
	@echo " INSTALL    SaveUnpack"
	@install SaveUnpack $(PREFIX)/bin
	@echo " INSTALL    SaveToProto"
	@install SaveToProto $(PREFIX)/bin
	@echo " INSTALL    SaveGenerate"
	@install SaveGenerate $(PREFIX)/bin
	@echo " INSTALL    ProtoToSave"
	@install ProtoToSave $(PREFIX)/bin
	@echo " INSTALL    ProfileUnpack"
	@install ProfileUnpack $(PREFIX)/bin
	@echo " INSTALL    ProfileGenerate"
	@install ProfileGenerate $(PREFIX)/bin
	@echo " INSTALL    CSaveGUI"
	@install CSaveGUI $(PREFIX)/bin
	@echo " INSTALL    CSaveMenu"
	@install CSaveMenu $(PREFIX)/bin
	@echo " INSTALL    ProtoToProfile"
	@install ProtoToProfile $(PREFIX)/bin
	@echo " INSTALL    ProfileToProto"
	@install ProfileToProto $(PREFIX)/bin
	@echo " INSTALL    SaveConvert"
	@install SaveConvert $(PREFIX)/bin

	

uninstall:
	@echo " RM    SaveUnpack"
	@rm $(PREFIX)/bin/SaveUnpack
	@echo " RM    SaveToProto"
	@rm $(PREFIX)/bin/SaveToProto
	@echo " RM    ProtoToSave"
	@rm $(PREFIX)/bin/ProtoToSave
	@echo " RM    SaveGenerate"
	@rm $(PREFIX)/bin/SaveGenerate
	@echo " RM    ProfileGenerate"
	@rm $(PREFIX)/bin/ProfileGenerate
	@echo " RM    ProfileUnpack"
	@rm $(PREFIX)/bin/ProfileUnpack
	@echo " RM    CSaveGUI"
	@rm $(PREFIX)/bin/CSaveGUI
	@echo " RM    CSaveMenu"
	@rm $(PREFIX)/bin/CSaveMenu
	@echo " RM    ProtoToProfile"
	@rm $(PREFIX)/bin/ProtoToProfile
	@echo " RM    ProfileToProto"
	@rm $(PREFIX)/bin/ProfileToProto
	@echo " RM    SaveConvert"
	@rm $(PREFIX)/bin/SaveConvert
	

.PHONY: clean

clean:
	rm -f *.o Oak*.pb-c.* *.a $(MANUALS_PS) $(MANUALS_PDF) $(MANUALS_TEXT) $(PROGS)
