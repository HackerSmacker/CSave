CC = gcc
LD = gcc
LDFLAGS = -lprotobuf-c -L/usr/local/lib -L/usr/lib
CFLAGS = -I.
DEPS = include/*.h
PROTOS = *.proto
OBJS = OakProfile.pb-c.o OakSave.pb-c.o OakShared.pb-c.o SaveUnpack.o FileTranslator.o SaveToProto.o ItemCode.o SaveGenerate.o
CPROTOS = OakProfile.pb-c.c OakSave.pb-c.c OakShared.pb-c.c
PROGS = SaveUnpack SaveToProto SaveGenerate
LIBRARIES = libBL3Proto.a
MANUALS_PS = manual.ps
MANUALS_PDF = manual.pdf

all: $(PROGS)

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
	@echo " LD    SaveGenerate"
	@$(LD) -o SaveGenerate SaveGenerate.o libBL3Proto.a $(LDFLAGS)

manual: $(MANUALS_PS) $(MANUALS_PDF)

.PHONY: clean

clean:
	rm -f *.o Oak*.pb-c.* *.a *.ps *.pdf SaveUnpack SaveToProto SaveGenerate
