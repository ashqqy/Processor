all: assembler processor

.PHONY: assembler processor

assembler:
	make -f ./assembler/MakeFile

processor:
	make -f ./SPU/MakeFile
