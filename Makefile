all: assembler processor

assembler:
	make -f ./Assembler/MakeFile

processor:
	make -f ./Processor/MakeFile