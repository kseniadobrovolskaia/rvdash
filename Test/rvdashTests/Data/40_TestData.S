# 40 Test: sll


.global _start

_start: addi a1, x0, -9
        addi a3, x0, 4
        sll a3, a1, a3  # a3 <- 0xfffffff70
        ebreak

