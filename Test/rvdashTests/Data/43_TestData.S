# 43 Test: slt


.global _start

_start: addi a1, x0, -18
        addi a3, x0, 2
        slt a3, a1, a3  # a3 <- 0x1
        ebreak

