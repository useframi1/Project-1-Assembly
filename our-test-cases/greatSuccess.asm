lui s0, 0x00010
addi t0, zero, 'G'
sb t0, 0(s0)
addi t0, zero, 'r'
sb t0, 1(s0)
addi t0, zero, 'e'
sb t0, 2(s0)
addi t0, zero, 'a'
sb t0, 3(s0)
addi t0, zero, 't'
sb t0, 4(s0)
addi t0, zero, ' '
sb t0, 5(s0)
addi t0, zero, 'S'
sb t0, 6(s0)
addi t0, zero, 'u'
sb t0, 7(s0)
addi t0, zero, 'c'
sb t0, 8(s0)
sb t0, 9(s0)
addi t0, zero, 'e'
sb t0, 10(s0)
addi t0, zero, 's'
sb t0, 11(s0)
sb t0, 12(s0)
addi t0, zero, '\0'
sb t0, 13(s0)
addi a7, zero, 4
add a0, zero, s0
ecall
addi a7, zero, 10
ecall