main:
	li a0, 9
	
	call add
	
	li, a7, 1
	ecall
	
	li, a7, 10
	ecall
	
add:	bnez a0, else
	li a0, 0
	ret 
else: 	addi sp, sp, -8
	sw ra,0(sp)
	sw a0,4(sp)
	
	addi a0,a0,-1
	call add
	
	lw t0,4(sp)
	add a0,a0,t0
	
	lw, ra,0(sp)
	addi sp,sp,8
	ret