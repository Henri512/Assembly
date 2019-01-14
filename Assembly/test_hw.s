# jednostavan "hello world" program

.global main, print

.text2
main:				# pocetak programa
	sub r2, r2		# r2 = 0
	ldr r3, r2, mbegin	# r3 = mdata, pocetak niza za ispis
	ldr r4, r2, mbound	# r4 = mend, kraj niza za ispis
	push r4			# stavljanje parametrara na stek
	push r3			# 
	call print		# poziv print
	add sp, 8		# skidanje sa steka

.data2
mbegin:	.long mdata		# pokazuje na pocetak niza
mbound:	.long mend		# pokazuje na kraj niza
mdata:	.char 'H', 'e', 'l', 'l', 'o', 0x2c, 0x20, 'w', 'o', 'r', 'l', 'd', '.', '\n'	# niz za ispis
mend:	.skip 3			# kraj niza

.text1
print:	
	push r11		# cuvanje koriscenih registara na steku
	push r12		# 
	push r13		# 
	push r14		# 
	push r15		# 
	
	sub r11, r11		# r11 = 0
	ldr r13, r11, output	# r13 = 0x1000, adresa za stampanje na standardni izlaz
	ldr r14, sp, 28		# r14 = mend, kraj niza
	ldr r15, sp, 24		# r15 = mdata, pocetak niza
	
loop:	cmp r15, r14		# poredjenje sa krajem
	je exit			# izlazak iz petlje
	ldr r12, r15		# r12 = mdata[r15], dohvatanje sledeceg znaka za ispis
	out r13, r12		# putchar(r12), ispis znaka
	add r15, r11, 1		# r15++, pomeranje na sledecu lokaciju
	jmp loop		# povratak na pocetak petlje

exit:	pop r15			# skidanje starih vrednosti registara sa steka
	pop r14			# 
	pop r13			# 
	pop r12			# 
	pop r11			# 
	ret			# povratak iz funkcije

.data1
output: .long 0x1000		# adresa za stampanje na standardni izlaz

.end main
