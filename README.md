# rvdash 

> **rvdash** - функциональная RISC-V модель, поддерживающая базовый набор инструкций RV32I.
> **rvdashSim** - cимулятор RISC-V, поддерживающий базовый набор инструкций RV32I.

> Полное название **RISC-V Dash** выбрано как у пони **Rainbow Dash** 
> за такую же страсть к победе и стремление к успеху :D!

 
![example](Pictures/rvdash.jpg) 


## Содержание

[1. Пример использования симулятора](#1)

Компиляция простого ассемблера в бинарный файл для симулятора


[2. Опции](#4)

Запуск симулятора

[3. Сборка ](#2)

Компиляция *rvdashSim*

[4. Тестирование](#3)

Запуск *GoogleTest*-ов




<a name="1"></a>
 ### Пример использования симулятора

Возьмём простой ассемблер, скомпилируем и запустим его на симуляторе.

```
cat Hello.S
```
  ```
  .global _start                    # Provide program starting address to linker

# Setup the parameters to print hellorvdash
# and then call Linux to do it.

_start:  addi  a0, x0, 1          # 1 = StdOut
           la    a1, hellorvdash    # load address of hellorvdash
           addi  a2, x0, 15         # length of string
           addi  a7, x0, 64         # linux write system call
           ecall                    # Call linux to output the string

# Setup the parameters to exit the program
# and then call Linux to do it.

           addi    a0, x0, 0        # Use 0 return code
           addi    a7, x0, 93       # Service command code 93 terminates
           ecall                    # Call linux to terminate the program

.data
hellorvdash:      .ascii   "Hello, rvdash!\n"

  ```

#### Компиляция
 
Для того, чтобы скомпилировать этот ассемблер нужен RISCV-тулчейн, а именно:

1. Ассемблер **riscv64-unknown-linux-gnu-as**
```
riscv64-unknown-linux-gnu-as -march=rv32i -mabi=ilp32 Hello.S -o Hello.o 
```
2. Линкер **riscv64-unknown-linux-gnu-ld**
```
riscv64-unknown-linux-gnu-ld -march=rv32i -m elf32lriscv_ilp32  Hello.o -o Hello.elf
```
3. Образ памяти **riscv64-unknown-linux-gnu-objcopy**
```
 riscv64-unknown-linux-gnu-objcopy -O binary Hello.elf Hello.bin
```

Можно также посмотреть как лежит программа в памяти:
```
xxd -c 4 -b Hello.bin
```
```
00000000: 00010011 00000101 00010000 00000000  ....            // addi
00000004: 10010111 00010101 00000000 00000000  ....            // auipc
00000008: 10010011 10000101 00000101 00000010  ....            // addi
0000000c: 00010011 00000110 11110000 00000000  ....            // addi
00000010: 10010011 00001000 00000000 00000100  ....            // addi
00000014: 01110011 00000000 00000000 00000000  s...            // ecall
00000018: 00010011 00000101 00000000 00000000  ....            // addi
0000001c: 10010011 00001000 11010000 00000101  ....            // addi
00000020: 01110011 00000000 00000000 00000000  s...            // ecall
...
00001020: 00000000 00000000 00000000 00000000  ....
00001024: 01001000 01100101 01101100 01101100  Hell
00001028: 01101111 00101100 00100000 01110010  o, r
0000102c: 01110110 01100100 01100001 01110011  vdas
00001030: 01101000 00100001 00001010           h!.

```

#### Запуск
 
 Симулятору *rvdashSim* отдаётся бинарный файл, а он его исполняет
 ```
 ./rvdashSim   Hello.bin
 ```
 Результатом выполнения является как трасса исполнения, так и вывод исполняемой программы:
 ```
 ====================Simulation started====================
addi X10, X0, 0x1
X10 <- 0x1
auipc X11, 0x1
X11 <- 0x1004
addi X11, X11, 0x20
X11 <- 0x1024
addi X12, X0, 0xf
X12 <- 0xf
addi X17, X0, 0x40
X17 <- 0x40
ecall write(1, 4132, 15)
Hello, rvdash!
addi X10, X0, 0x0
X10 <- 0x0
addi X17, X0, 0x5d
X17 <- 0x5d
ecall exit(0)
===================Simulation completed===================

 ``` 
 
 Чтобы трасса исполнения записывалась не в стандартный поток вывода, а в файл, например, **trace.txt** 
 можно указать его опцией  **-t**.
 
-----------------------------------------------------------------------------

 
 <a name="4"></a>
 ### Опции
 
 Все доступные опции можно узнать:
 ```
 ./rvdashSim   --help
 ```
```
USAGE:   ./rvdashSim   [options]   <binary_file>

OPTIONS: 
	    -h	 --help
	    -r	 --ram-size
	    -p	 --program-counter
	    -t	 --trace-output
```

* **--help**, **-h** - распечатать справку
* **--ram-size**, **-r** - задать размер виртуальной памяти (в Мегабайтах), она пока что вся произвольного доступа:)
* **--program-counter**, **-p** - задать начальное значение регистра Program counter (в байтах). Это значение должно быть выровнено по размеру инструкции, то есть для RV32I должно быть кратно 4-м байтам.
* **--trace-output**, **-t** - задать файл, для печати трассы исполнения.

### Запуск с использованием опций
 
 Симулятору *rvdashSim* отдаётся бинарный файл и опции в любом порядке:
 ```
 ./rvdashSim  --trace-output trace.txt   Hello.bin   --ram-size 1    -p 0
 ```
 Результатом выполнения является сгенерированная в файл  **trace.txt** трасса и вывод исполняемой программы на экране:
 ```
Hello, rvdash!
```


 
-----------------------------------------------------------------------------

<a name="2"></a>
 ### Сборка

```
  $git clone https://github.com/kseniadobrovolskaia/rvdash
  $cd rvdash/
  $cmake -B build
  $cd build/
  $make
```
 
#### Симуляция

Чтобы выполнить скомпилированную RISCV-тулчейном программу **Prog.bin** на *симуляторе rvdashSim* нужно в директории `build`:
``` 
  $./rvdashSim   Prog.bin   -t trace.txt
``` 

Трасса исполнения будет записана в файл **trace.txt** в директории `build`


-----------------------------------------------------------------------------

<a name="3"></a>
 ### Тестирование

Для запуска тестов должны быть установлены и находиться в **PATH** следующие программы
(либо запуск происходит в контейнере, где они есть :) ):

Для компиляции тестов:
* **riscv64-unknown-linux-gnu-as**
* **riscv64-unknown-linux-gnu-ld**
* **riscv64-unknown-linux-gnu-objcopy**

Для проверки трасс:
* **FileCheck**

Чтобы запустить *тестирование модели rvdash* нужно, находясь в `build`:  

```
  $cd Test/
  $./rvdashTests
```
С использованием тестового фреймворка **GoogleTest** будут запущены тесты из `rvdash/Test/rvdashTests/Data`, `rvdash/Test/ErrorHandlingTests/Data`. Результаты тестирования будут на экране.

```
...
[ RUN      ] Test_rvdash.Test44
[       OK ] Test_rvdash.Test44 (3 ms)
[----------] 44 tests from Test_rvdash (165 ms total)


[==========] 47 tests from 2 test suites ran. (440 ms total)
[  PASSED  ] 47 tests.
```
 
 
