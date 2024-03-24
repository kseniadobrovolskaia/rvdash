# 6 Test: slli


.global _start

_start: addi a1, x0, -9
        slli a3, a1, 4  # a3 <- 0xfffffff70
        ebreak

