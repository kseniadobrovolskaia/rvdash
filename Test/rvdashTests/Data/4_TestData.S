# 4 Test: ori


.global _start

_start: addi a1, x0, 93
        ori a3, a1, 887  # a3 <- 0x37f
        ebreak

