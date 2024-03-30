# 6 Test: Address exceeds ram size


.global _start

_start: lui x1, 0x1000
        sw x0, 0x100(x1)
        ebreak

