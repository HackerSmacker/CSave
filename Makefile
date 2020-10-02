CC = gcc
LD = gcc
LDFLAGS = -lprotobuf-c
CFLAGS = -I.
DEPS = include/*.h
PROTOS = *.proto
OBJS = OakProfile.pb-c.o OakSave.pb-c.o OakShared.pb-c.o SaveUnpack.o FileTranslator.o SaveToProto.o ItemCode.o SaveGenerate.o
CPROTOS = OakProfile.pb-c.c OakSave.pb-c.c OakShared.pb-c.c
PROGS = SaveUnpack SaveToProto SaveGenerate

%.o: %.c
	@echo " CC  " $<
	@$(CC) $(CFLAGS) -c -o $@ $<

%.pb-c.c: %.proto
	@echo " PROTOC  " $<
	@protoc-c --c_out=. $<
	

$(PROGS): $(CPROTOS) $(OBJS)
	@echo " LD  SaveUnpack"
	@$(LD) -o SaveUnpack SaveUnpack.o OakSave.pb-c.o OakShared.pb-c.o FileTranslator.o $(LDFLAGS)
	@echo " LD  SaveToProto"
	@$(LD) -o SaveToProto SaveToProto.o OakSave.pb-c.o OakShared.pb-c.o FileTranslator.o $(LDFLAGS)
	@echo " LD  SaveGenerate"
	@$(LD) -o SaveGenerate SaveGenerate.o OakSave.pb-c.o OakShared.pb-c.o FileTranslator.o $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o Oak*.pb-c.* SaveUnpack SaveToProto SaveGenerate
