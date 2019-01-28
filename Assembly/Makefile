CFLAGS=-std=gnu99

BINARIES= main.o constants_helper.o helpers.o string_helpers.o sections.o parser_helpers.o section_helpers.o directives.o instruction_helpers.o instructions.o parser_first.o parser_second.o object_file_writer.o

assembler: $(BINARIES)
	gcc $(CFLAGS) $(BINARIES) -lm -o assembler

main.o: main.c constants_helper.h helpers.h string_helpers.h sections.h parser_helpers.h section_helpers.h directives.h instruction_helpers.h instructions.h parser_first.h parser_second.h object_file_writer.h
	gcc $(CFLAGS) -lm -c main.c

constants_helper.o: constants_helper.c constants_helper.h
	gcc $(CFLAGS) -c constants_helper.c

helpers.o: helpers.c helpers.h
	gcc $(CFLAGS) -c helpers.c

string_helpers.o: string_helpers.c string_helpers.h
	gcc $(CFLAGS) -c string_helpers.c

sections.o: sections.c sections.h
	gcc $(CFLAGS) -c sections.c

parser_helpers.o: parser_helpers.c parser_helpers.h
	gcc $(CFLAGS) -c parser_helpers.c

section_helpers.o: section_helpers.c section_helpers.h
	gcc $(CFLAGS) -c section_helpers.c

directives.o: directives.c directives.h
	gcc $(CFLAGS) -c -lm directives.c

instruction_helpers.o: instruction_helpers.c instruction_helpers.h
	gcc $(CFLAGS) -c instruction_helpers.c

instructions.o: instructions.c instructions.h
	gcc $(CFLAGS) -c instructions.c

parser_first.o: parser_first.c parser_first.h
	gcc $(CFLAGS) -c parser_first.c

parser_second.o: parser_second.c parser_second.h
	gcc $(CFLAGS) -c parser_second.c

object_file_writer.o: object_file_writer.c object_file_writer.h
	gcc $(CFLAGS) -c object_file_writer.c

all: clean $(binaries)

clean:
	rm -f $(binaries) *.o
