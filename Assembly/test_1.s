# jednostavan "hello world" program

.global main, print

.data
mbegin:	.long mdata		# pokazuje na pocetak niza
mbound:	.long mend		# pokazuje na kraj niza
mdata:	.char 'H', 'e', 'l', 'l', 'o', 0x2c, 0x20, 'w', 'o', 'r', 'l', 'd', '.', '\n'	# niz za ispis
mend:	.skip 3			# kraj niza
output: .long 0xFFFE	# adresa za stampanje na standardni izlaz
.align 8

.text
main:				# pocetak programa
	jmp *50
	jmp r7[mend]
	#ret 1
	call r4[mdata]
	push $20
	push $mdata
	pop r0
	pop $20
	push $unknown
	sub r0, r0		# r0 = 0
	mov r1, r0[mbegin]	# r1 = mdata, pocetak niza za ispis
	mov r2, r0[mbound]	# r2 = mend, kraj niza za ispis
	push r2			# stavljanje parametrara na stek
	push r1			# 
	call print		# poziv print
	add sp, 4		# skidanje sa steka

print:	
	push r0		# cuvanje koriscenih registara na steku
	push r1		# 
	push r2		# 
	push r3		# 
	push r4		# 
	
	sub r0, r0			# r0 = 0
	mov r2, r0[output]	# r2 = 0xFFFE, adresa za stampanje na standardni izlaz
	mov r3, sp[14]		# r3 = mend, kraj niza
	mov r4, sp[12]		# r4 = mdata, pocetak niza
	
loop:	
	cmp r4, r3	# poredjenje sa krajem
	jmp_eq exit			# izlazak iz petlje
	mov r1, r4		# r1 = mdata[r4], dohvatanje sledeceg znaka za ispis
	push r1
	call printf		# printf(r1), ispis znaka
	add r4, 1		# r4++, pomeranje na sledecu lokaciju
	jmp loop		# povratak na pocetak petlje
	mov r1, &psw	
	mov_ne r1, r2
	mov_gt r3, r4

exit:	
	pop r4		# skidanje starih vrednosti registara sa steka
	pop r3			# 
	pop r2			# 
	pop r1			# 
	pop r0			# 
	ret			    # povratak iz funkcije

.end

mov r1, psw			# 00 1101 01 001 00 111