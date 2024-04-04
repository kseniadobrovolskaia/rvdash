# rvdash 

> [!NOTE]
> Полное название **RISC-V Dash** выбрано как у пони **Rainbow Dash** 
> за такую же страсть к победе и стремление к успеху! :smile:



> [!IMPORTANT]
> * **rvdash** - функциональная RISC-V модель, поддерживающая базовый набор инструкций RV32I.
> * **rvdashSim** - cимулятор RISC-V на основе *rvdash*.
> * **SnippyRVdash** - библиотека на основе *rvdash*, совместимая с тестовым генератором *llvm-snippy*.


 
![example](Pictures/rvdash.jpg) 


## Содержание

[1. Пример использования симулятора](#1)

Компиляция простого ассемблера в бинарный файл для симулятора *rvdashSim*

[2. Опции](#4)

Запуск симулятора

[3. Модель для Snippy](#5)

Использование библиотеки модели *SnippyRVdash* вместе с тестовым генератором *llvm-snippy*

[4. Сборка ](#2)

Компиляция *rvdashSim*, *SnippyRVdash* и тестов 

[5. Тестирование](#3)

Запуск *GoogleTest*-ов




<a name="1"></a>
 ## Пример использования симулятора

Возьмём простой ассемблер, скомпилируем и запустим его на симуляторе.

```
$ cat Hello.S
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
$ riscv64-unknown-linux-gnu-as -march=rv32i -mabi=ilp32 Hello.S -o Hello.o 
```
2. Линкер **riscv64-unknown-linux-gnu-ld**
```
$ riscv64-unknown-linux-gnu-ld -march=rv32i -m elf32lriscv_ilp32  Hello.o -o Hello.elf
```
3. Образ памяти **riscv64-unknown-linux-gnu-objcopy**
```
$ riscv64-unknown-linux-gnu-objcopy -O binary Hello.elf Hello.bin
```

Можно также посмотреть как лежит программа в памяти:
```
$ xxd -c 4 -b Hello.bin
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
$ ./rvdashSim   Hello.bin
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
 ## Опции
 
 Все доступные опции можно узнать:
 ```
$ ./rvdashSim   --help
 ```
```
USAGE:      ./rvdashSim   [options]   <binary_file>

OPTIONS: 
	    -h	 --help
	         --ram-start
	         --ram-size
	    -p	 --program-counter
	    -t	 --trace-output
```


| Опция  | Алиас |Описание         |
|:------------- |:----------|:----------------------------:|
|  **--help**       |  **-h**          | Распечатать справку.|
| **--ram-start**        |          |Задать начальный адрес виртуальной памяти произвольного доступа (в байтах). Значение по умолчанию 0.|
| **--ram-size**        |          |Задать размер виртуальной памяти (в MB), она пока что вся произвольного доступа:). Значение по умолчанию 1 MB.|
| **--program-counter**       |  **-p**         | Задать начальное значение регистра Program counter (в байтах). Это число должно быть выровнено по размеру инструкции, то есть для RV32I должно быть кратно 4-м байтам. Значение по умолчанию 0.|
| **--trace-output**      |  **-t**         | Задать файл, для печати трассы исполнения. Без указания трасса печатается на экране.|


#### Запуск с использованием опций
 
 Симулятору *rvdashSim* отдаётся бинарный файл и опции в любом порядке:
 ```
$ ./rvdashSim  --trace-output trace.txt   Hello.bin   --ram-size 1    -p 0
 ```
 Результатом выполнения является сгенерированная в файл  **trace.txt** трасса и вывод исполняемой программы на экране:
 
```
Hello, rvdash!
```

-----------------------------------------------------------------------------


 <a name="5"></a>
 ## Модель для Snippy


> [!NOTE]
> **LLVM-snippy** - кроссплатформенный генератор случайного кода. Он поддерживает создание циклов, вызовов функций, шаблонов памяти и многое другое.
>
> **Репозиторий Snippy**: [https://github.com/syntacore/snippy]


#### Простейший запуск Snippy

Простейший вариант запуска Snippy:

```
 $ ./llvm-snippy   Simple_layout.yaml
```

Где конфигурационный файл **Simple_layout.yaml** задает адреса для размещения кода в памяти и обращений к ней, количество и названия инструкций и многое другое. 

В простейшем случае, если, например, требуется сгенерировать 5 инструкций **add**:

```
 $ cat   Simple_layout.yaml
```

```
options:
  march: riscv32
  num-instrs: 5
  model-plugin: None
  reserved-regs-list: [ra, gp]

sections:
  - name:      text
    VMA:       0x1000
    SIZE:      0x10000
    LMA:       0x1000
    ACCESS:    rx
  - name:      data
    VMA:       0x11000
    SIZE:      0x10000
    LMA:       0x11000
    ACCESS:    rw

histogram:
    - [ADD, 1.0]
```

Snippy сгенерирует файл  **Simple_layout.yaml.elf**, содержащий одну функцию SnippyFunction с пятью инструкциями add и линкер скрипт **Simple_layout.yaml.ld**.

Можно посмотреть ассемблер сгенерированного кода:

```
 $ riscv64-unknown-linux-gnu-objdump -D Simple_layout.yaml.elf
```
```
Simple_layout.yaml.elf:     file format elf32-littleriscv


Disassembly of section .snippy.text.rx:

00000000 <SnippyFunction>:
   0:	011a84b3          	add	s1,s5,a7
   4:	002d8733          	add	a4,s11,sp
   8:	01750433          	add	s0,a0,s7
   c:	01ed8333          	add	t1,s11,t5
  10:	00e30033          	add	zero,t1,a4

00000014 <__snippy_exit>:
  14:	00100073          	ebreak  

```

#### Опция Snippy --model-plugin

Во время генерации случайного кода может быть полезным узнать состояние регистров и памяти. 
Например, может потребоваться закончить генерацию, если во всех регистрах переполнение NAN или нули. 
Snippy - генератор, поэтому он не выполняет сгенерированные инструкции, но у него есть поддержка моделей. 

> [!TIP]
> **Модель для Snippy** - это динамическая библиотека, которая подается программе *llvm-snippy* при её запуске в качестве опции **--model-plugin**

После запуска Snippy подгружает эту библиотеку и с её помощью выполняет сгенерированный код.

#### Модель SnippyRVdash

> [!IMPORTANT]
> У rvdash также есть обертка для совместимости со Snippy. Это динамическая библиотека **libSnippyRVdash.so**, путь до которой нужно указать при запуске *llvm-snippy*. 


Если *llvm-snippy* и *libSnippyRVdash.so* находятся в одной директории, то достаточно указать её в конфигурационном файле **Model_layout.yaml**:


```
 $ cat   Model_layout.yaml
```

```
options:
  march: riscv32
  num-instrs: 5
  model-plugin: libSnippyRVdash.so
  reserved-regs-list: [ra, gp]

sections:
  - name:      text
    VMA:       0x1000
    SIZE:      0x10000
    LMA:       0x1000
    ACCESS:    rx
  - name:      data
    VMA:       0x11000
    SIZE:      0x10000
    LMA:       0x11000
    ACCESS:    rw

histogram:
    - [ADD, 1.0]
```


Теперь, находясь в директории, где установлена *llvm-snippy* (а также обязательно линкер ld.lld) и лежит для неё конфигурационный файл **Model_layout.yaml**:

```
  $ ./llvm-snippy   Model_layout.yaml
```


Результатом выполнения теперь помимо *elf* и *ld* файлов является трасса исполнения на указанной в опции *--model-plugin* модели.
В нашем случае на модели **SnippyRVdash**:

```
warning: no instructions seed specified, using auto-generated one: 1712081567869574432
ld.lld: warning: cannot find entry symbol _start; not setting start address
====================rvdash start====================
NOTE: adjusting MSTATUS: 0x0->0
add X13, X19, X24
X13 <- 0x311fc9ce
add X25, X27, X24
X25 <- 0xeb80a17c
add X10, X27, X2
X10 <- 0x7aafb1fa
add X0, X2, X17
X0 <- 0x8a431913
add X29, X20, X11
X29 <- 0x1bac59fd
ebreak
remark: Snippet generator generated relocatable image: please, use linker with provided script to generate final image
===================rvdash complete==================

```

-----------------------------------------------------------------------------


<a name="2"></a>
 ## Сборка
 
 Сначала нужно склонировать репозиторий:

```
  $ git clone https://github.com/kseniadobrovolskaia/rvdash
  $ cd rvdash/
```

##### Затем есть несколько вариантов сборки, в зависимости от того, что требуется.
 
* По умолчанию будут скомпилированы все программы: симулятор *rvdashSim*, модель для Snippy *SnippyRVdash* и тесты *GoogleTests*:

```
  $ cmake   -B build
  $ cd build/
  $ make
```

* Если модель *SnippyRVdash* не требуется, то нужно указать при запуске *cmake*:

```
  $ cmake   -DBUILD_SNIPPY_MODEL=OFF   -B build
```

* Если не требуется сборка тестов, то нужно указать при запуске *cmake*:

```
  $ cmake   -DBUILD_TESTS=OFF   -B build
```

* Если нужно собрать только симулятор, без модели для Snippy и без тестов:

```
  $ cmake   -DBUILD_TESTS=OFF -DBUILD_SNIPPY_MODEL=OFF   -B build
```

 
#### Симуляция

Чтобы выполнить скомпилированную RISCV-тулчейном программу **Prog.bin** на *симуляторе rvdashSim* нужно в директории `build`:
``` 
  $ ./rvdashSim   Prog.bin   -t trace.txt
``` 

Трасса исполнения будет записана в файл **trace.txt** в директории `build`


#### Модель для Snippy

> [!IMPORTANT]
>После компиляции (без отключения модели) в директории `build/lib/SnippyRVdash` будет находиться модель в виде динамической библиотеки **libSnippyRVdash.so**, путь до которой нужно указать в конфигурационном файле *layout.yaml* для Snippy. Его полный вид есть в [примере](#5).

Если репозиторий rvdash был склонирован в директорию с абсолютным путем **PathToDir**, то в файле *layout.yaml*:

``` 
...
options:

  --model-plugin: PathToDir/rvdash/build/lib/SnippyRVdash/libSnippyRVdash.so
...
```

Теперь, находясь в директории, где установлена *llvm-snippy* и лежит для неё конфигурационный файл **layout.yaml**:

```
  $ ./llvm-snippy   layout.yaml
``` 

Snippy использует эту модель для выполнения сгенерированных инструкций.


-----------------------------------------------------------------------------

<a name="3"></a>
 ## Тестирование

> [!IMPORTANT]
>Для запуска тестов должны быть установлены и находиться в **PATH** следующие программы
>(либо запуск происходит в контейнере, где они есть :smile: ):

Для компиляции тестов:
* **riscv64-unknown-linux-gnu-as**
* **riscv64-unknown-linux-gnu-ld**
* **riscv64-unknown-linux-gnu-objcopy**

Для проверки трасс:
* **FileCheck**

Чтобы запустить *тестирование модели rvdash* нужно, находясь в `build`:  

```
  $ cd Test/
  $ ./rvdashTests
```
С использованием тестового фреймворка **GoogleTest** будут запущены тесты из 
`rvdash/Test/rvdashTests/Data`, `rvdash/Test/ErrorHandlingTests/Data`. 

Результаты тестирования будут на экране:

```
...
[ RUN      ] Test_rvdash.Test45
[       OK ] Test_rvdash.Test45 (3 ms)
[----------] 45 tests from Test_rvdash (165 ms total)


[==========] 51 tests from 2 test suites ran. (440 ms total)
[  PASSED  ] 51 tests.
```
 
 
