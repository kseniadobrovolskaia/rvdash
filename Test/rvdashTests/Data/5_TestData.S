# 5 Test: andi


.global _start

_start: addi a1, x0, -1
        andi a3, a1, 37  # a3 <- 0x25
        ebreak

