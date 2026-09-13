    .text
    .globl add_with_bias
    .type add_with_bias, @function
add_with_bias:
    addi sp, sp, -16
    sd ra, 8(sp)
    sd s0, 0(sp)
    li s0, 5
    call add_pair
    add a0, a0, s0
    ld s0, 0(sp)
    ld ra, 8(sp)
    addi sp, sp, 16
    ret
    .size add_with_bias, .-add_with_bias

    .type add_pair, @function
add_pair:
    add a0, a0, a1
    ret
    .size add_pair, .-add_pair
    .section .note.GNU-stack,"",@progbits
