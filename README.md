Asembler:
Priloženi izvršni program, izvorni kod i testovi predstavljaju realizaciju asemblerskog dela projektnog zadatka iz sistemskog softvera za februar 2019. godine.
Zadatak je zahtevao da se napravi dvoprolazni asembler za procesor opisan u postavci zadatka.
Rešenje je realizovano u potpunosti na programskom jeziku C. Izvorni kod je podeljen u nekoliko .c i .h datoteka koje obavljaju glavne stvari, parsiranje, citanje i upis generisanog koda u fajl kao i nekoliko .c i .h fajlova koji služe kao pomoć pri nekim često ponavljanim operacijama (_helpers fajlovi).
Lista svih datoteka:
main.c – sadrzi glavnu funkciju koja otvara fajlove i poziva sve ostale funkcije
object_file_writer.h
object_file_writer.c – sadrži funkcije za ispis tabele simbola i sekcija u izlazni fajl
parser_second.h
parser_second.c – funkcije za parsiranje ulaznog fajla u drugom prolazu
parser_first.h
parser_first.c  – funkcije za parsiranje ulaznog fajla u prvom prolazu
instructions.h
instructions.c – funkcije za parsiranje instrukcija u oba prolaza
directives.h
directive.c – funkcije za parsiranje direktiva u oba prolaza
sections.h
sections.c – funkcije za operacije sa tabelom simbola i sekcijama, definicije struktura sekcija i tabele simbola

constants.h – definija konstanti i enumeracija koriscenih u parsiranju
helper fajlovi (.h i .c): helpers, string_helpers, parser_helpers, section_helpers, instruction_helpers, 

Program prvo otvara ulazni fajl standardnim C funkcijama, koji se potom parsira (funkcija executeFirstPass iz parser_first.c) gde se sve naredbe obrađuju u kako je definisano u prvom prolazu asemblera. Zatim se pozivom metode resetCounters iz section_helpers.c fajla resetuju brojači sekcija i onda se počinje drugi prolaz asemblera pozivom funkcije executeSecondPass iz parser_second.c fajla. Nakon izvršavanja drugog prolaza sve je spremno za upis i pozivom fukncije writeDataToFile iz object_file_writer.c fajla se generisani mašinski kod upisuje i uzlazni fajl po ELF formatu. Time se dobija .o fajl koji predstavlja objektni fajl u tekstualnom obliku za ulazni .s fajl.
Priložen je i Makefile kojim se rešenje prevodi i povezuje. Stoga opcija za prevođenje glasi:
make 
a pocija za brisanje generisanih fajlova:
make clean
Izvršni program se pokreće tako sto se pozove sa nazivom ulaznog fajla kao parametrom:
./assembler test.s
