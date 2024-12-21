all: assembler processor

.PHONY: assembler processor

assembler:
	make -f ./Assembler/MakeFile

processor:
	make -f ./SPU/MakeFile