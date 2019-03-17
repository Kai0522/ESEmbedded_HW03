HW03
===
This is the hw03 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw03 dir, use:

	* `make` to build.

	* `make clean` to clean the ouput files.

4. Extract `gnu-mcu-eclipse-qemu.zip` into hw03 dir. Under the path of hw03, start emulation with `make qemu`.

	See [Lecture 02 ─ Emulation with QEMU] for more details.

5. The sample is a minimal program for ARM Cortex-M4 devices, which enters `while(1);` after reset. Use gdb to get more details.

	See [ESEmbedded_HW02_Example] for knowing how to do the observation and how to use markdown for taking notes.

# Build Your Own Program

1. Edit main.c.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C Standard library functions like `printf`, `malloc`, etc.

# HW03 Requirements

1. How do C functions pass and return parameters? Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.

2. Modify main.c to observe what you found.

3. You have to state how you designed the observation (code), and how you performed it.

	Just like how you did in HW02.

3. If there are any official data that define the rules, you can also use them as references.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

[Lecture 02 ─ Emulation with QEMU]: http://www.nc.es.ncku.edu.tw/course/embedded/02/#Emulation-with-QEMU
[ESEmbedded_HW02_Example]: https://github.com/vwxyzjimmy/ESEmbedded_HW02_Example

--------------------

 **If you volunteer to give the presentation next week, check this.**

--------------------
HW02 
===
## 1. 實驗題目
改寫程式以演示Return指令如何運行。
## 2. 實驗內容
在執行副程式呼叫時,必須有一個固定的協議(Calling convention)可以遵守以避免Caller與Callee對於資料存放位置有不同的規範,導致資料遺失或存放漫無章法的問題,另外此處針對堆疊的清理是交由副程式執行,為一個類似pascal calling convention或stdcall的呼叫形式.
但事實上ARM架構有獨有的Calling convention叫作AAPCS,以下是官方文件Procedure Call Standard for the ARM Architecture對此協議的介紹:
![](https://github.com/Kai0522/ESEmbedded_HW03/blob/master/AAPCS/feature.png)
![](https://github.com/Kai0522/ESEmbedded_HW03/blob/master/AAPCS/Register.png)
![](https://github.com/Kai0522/ESEmbedded_HW03/blob/master/AAPCS/Memory.png)
以上文件對此協議有一個介紹,所有的caller與callee皆會以上述的規範進行存放與使用記憶體

## 3. 實驗步驟

1. 設計測試程式 main.c ，從 _rest_handler) 開始後依序執行(a)引入一個引數並回傳一個值(b)引入五個引數並回傳一個值(c)引入五個指標引數，併使用指標回傳複數個值。


main.c:

```c
int OneArgOneReturn(int a){
    int b=a+200;
    return b;
}

int FiveArgOneReturn(int a,int b,int c,int d,int e){
    int f=(a+b)*c+d+e;
    return f;
}

void MutiReturn(int *a0,int *a1,int *a2,int *a3,int *a4){
        a0=a0+100;
        a1=a1+100;
        a2=a2+100;
        a3=a3+100;
        a4=a4+100;
}

void reset_handler(void){
    int a0=0;
    int a1=1;
    int a2=2;
    int a3=3;
    int a4=4;

    int x= OneArgOneReturn(1);
    int y=FiveArgOneReturn(1,2,3,4,5);
    MutiReturn(&a0,&a1,&a2,&a3,&a4);
	while (1)
		;
}
```

並將 main.c 編譯,取得其反組譯碼如下：  
```
Disassembly of section .mytext:

00000000 <OneArgOneReturn-0x8>:
   0:	20000100 	andcs	r0, r0, r0, lsl #2
   4:	00000099 	muleq	r0, r9, r0

00000008 <OneArgOneReturn>:
   8:	b480      	push	{r7}
   a:	b085      	sub	sp, #20
   c:	af00      	add	r7, sp, #0
   e:	6078      	str	r0, [r7, #4]
  10:	687b      	ldr	r3, [r7, #4]
  12:	33c8      	adds	r3, #200	; 0xc8
  14:	60fb      	str	r3, [r7, #12]
  16:	68fb      	ldr	r3, [r7, #12]
  18:	4618      	mov	r0, r3
  1a:	3714      	adds	r7, #20
  1c:	46bd      	mov	sp, r7
  1e:	f85d 7b04 	ldr.w	r7, [sp], #4
  22:	4770      	bx	lr

00000024 <FiveArgOneReturn>:
  24:	b480      	push	{r7}
  26:	b087      	sub	sp, #28
  28:	af00      	add	r7, sp, #0
  2a:	60f8      	str	r0, [r7, #12]
  2c:	60b9      	str	r1, [r7, #8]
  2e:	607a      	str	r2, [r7, #4]
  30:	603b      	str	r3, [r7, #0]
  32:	68fa      	ldr	r2, [r7, #12]
  34:	68bb      	ldr	r3, [r7, #8]
  36:	4413      	add	r3, r2
  38:	687a      	ldr	r2, [r7, #4]
  3a:	fb02 f203 	mul.w	r2, r2, r3
  3e:	683b      	ldr	r3, [r7, #0]
  40:	441a      	add	r2, r3
  42:	6a3b      	ldr	r3, [r7, #32]
  44:	4413      	add	r3, r2
  46:	617b      	str	r3, [r7, #20]
  48:	697b      	ldr	r3, [r7, #20]
  4a:	4618      	mov	r0, r3
  4c:	371c      	adds	r7, #28
  4e:	46bd      	mov	sp, r7
  50:	f85d 7b04 	ldr.w	r7, [sp], #4
  54:	4770      	bx	lr
  56:	bf00      	nop

00000058 <MutiReturn>:
  58:	b480      	push	{r7}
  5a:	b085      	sub	sp, #20
  5c:	af00      	add	r7, sp, #0
  5e:	60f8      	str	r0, [r7, #12]
  60:	60b9      	str	r1, [r7, #8]
  62:	607a      	str	r2, [r7, #4]
  64:	603b      	str	r3, [r7, #0]
  66:	68fb      	ldr	r3, [r7, #12]
  68:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  6c:	60fb      	str	r3, [r7, #12]
  6e:	68bb      	ldr	r3, [r7, #8]
  70:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  74:	60bb      	str	r3, [r7, #8]
  76:	687b      	ldr	r3, [r7, #4]
  78:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  7c:	607b      	str	r3, [r7, #4]
  7e:	683b      	ldr	r3, [r7, #0]
  80:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  84:	603b      	str	r3, [r7, #0]
  86:	69bb      	ldr	r3, [r7, #24]
  88:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  8c:	61bb      	str	r3, [r7, #24]
  8e:	3714      	adds	r7, #20
  90:	46bd      	mov	sp, r7
  92:	f85d 7b04 	ldr.w	r7, [sp], #4
  96:	4770      	bx	lr

00000098 <reset_handler>:
  98:	b590      	push	{r4, r7, lr}
  9a:	b08b      	sub	sp, #44	; 0x2c
  9c:	af02      	add	r7, sp, #8
  9e:	2300      	movs	r3, #0
  a0:	617b      	str	r3, [r7, #20]
  a2:	2301      	movs	r3, #1
  a4:	613b      	str	r3, [r7, #16]
  a6:	2302      	movs	r3, #2
  a8:	60fb      	str	r3, [r7, #12]
  aa:	2303      	movs	r3, #3
  ac:	60bb      	str	r3, [r7, #8]
  ae:	2304      	movs	r3, #4
  b0:	607b      	str	r3, [r7, #4]
  b2:	2001      	movs	r0, #1
  b4:	f7ff ffa8 	bl	8 <OneArgOneReturn>
  b8:	61f8      	str	r0, [r7, #28]
  ba:	2305      	movs	r3, #5
  bc:	9300      	str	r3, [sp, #0]
  be:	2001      	movs	r0, #1
  c0:	2102      	movs	r1, #2
  c2:	2203      	movs	r2, #3
  c4:	2304      	movs	r3, #4
  c6:	f7ff ffad 	bl	24 <FiveArgOneReturn>
  ca:	61b8      	str	r0, [r7, #24]
  cc:	f107 0014 	add.w	r0, r7, #20
  d0:	f107 0110 	add.w	r1, r7, #16
  d4:	f107 020c 	add.w	r2, r7, #12
  d8:	f107 0408 	add.w	r4, r7, #8
  dc:	1d3b      	adds	r3, r7, #4
  de:	9300      	str	r3, [sp, #0]
  e0:	4623      	mov	r3, r4
  e2:	f7ff ffb9 	bl	58 <MutiReturn>
  e6:	e7fe      	b.n	e6 <reset_handler+0x4e>
```

此時可以看到原先反向push與pop的指令接被assembler自動轉換為遞增順序，由此可以確定使用push與pop指令並無法顛倒順序反向送入與取出stack。  
2.先以第一個呼叫討論主程式如何執行call function:
```	
  9c:	af02      	add	r7, sp, #8
  9e:	2300      	movs	r3, #0
  a0:	617b      	str	r3, [r7, #20]
 		.
 		.
 		.
  b2:	2001      	movs	r0, #1
  b4:	f7ff ffa8 	bl	8 <OneArgOneReturn>
  b8:	61f8      	str	r0, [r7, #28]
```
可以看到主程式中R7接被用來存放堆疊記憶體的位址,R3皆優先用於暫存運算的結果,且在呼叫副程式前會先將引數傳到R0做存放,回傳後會透過R7進行指向將副程式運算的結果從記憶體放回R0
此時觀察第一個副程式如何運算一個指標一個回傳:
```
   8:	b480      	push	{r7}
   a:	b085      	sub	sp, #20
   c:	af00      	add	r7, sp, #0
   e:	6078      	str	r0, [r7, #4]
  10:	687b      	ldr	r3, [r7, #4]
  12:	33c8      	adds	r3, #200	; 0xc8
  14:	60fb      	str	r3, [r7, #12]
  16:	68fb      	ldr	r3, [r7, #12]
  18:	4618      	mov	r0, r3
  1a:	3714      	adds	r7, #20
  1c:	46bd      	mov	sp, r7
  1e:	f85d 7b04 	ldr.w	r7, [sp], #4
  22:	4770      	bx	lr
  ```
以下將內容以文字化說明其執行之工作:  
(1)將R7放入堆疊記憶體,運算完後放回R7,以免影響主程式的運算  
(2)將現在的SP向下推20以取得空白的堆疊記憶體執行副程式  
(3)將目前的SP取出放至R7以進行副程式之運算  
(4-5)將主程式輸入之引數R0放入R7+4的位置並取出至R3以進行後續運算  
(6)執行副程式中的運算  
(7-9)將運算結果存入R7+12並將資料放至R0準備回傳  
(10-11)將SP往上推20放回原先位置  
(12)使用後置索引,將第一步放入堆疊的資料取回R7,以回歸原先的堆疊指標,並將SP推回執行第一步前的位置,避免堆疊溢位  
(13)指回主程式  
3.以下在接著討論當引數數量較多時會如何進行資料的傳遞,先討論主程式的運作方式
```
  ba:	2305      	movs	r3, #5
  bc:	9300      	str	r3, [sp, #0]
  be:	2001      	movs	r0, #1
  c0:	2102      	movs	r1, #2
  c2:	2203      	movs	r2, #3
  c4:	2304      	movs	r3, #4
  c6:	f7ff ffad 	bl	24 <FiveArgOneReturn>
  ca:	61b8      	str	r0, [r7, #24]
```
由上述反組譯碼可以看到當數量大於4個時,就不會使用Register進行資料傳遞,反之會將大於四筆的資料會先被放入堆疊記憶體中.以下在從副程式觀察,此情形發生時副程式會如何對應:
```
  24:	b480      	push	{r7}
  26:	b087      	sub	sp, #28
  28:	af00      	add	r7, sp, #0
  2a:	60f8      	str	r0, [r7, #12]
  2c:	60b9      	str	r1, [r7, #8]
  2e:	607a      	str	r2, [r7, #4]
  30:	603b      	str	r3, [r7, #0]
  32:	68fa      	ldr	r2, [r7, #12]
  34:	68bb      	ldr	r3, [r7, #8]
  36:	4413      	add	r3, r2
  38:	687a      	ldr	r2, [r7, #4]
  3a:	fb02 f203 	mul.w	r2, r2, r3
  3e:	683b      	ldr	r3, [r7, #0]
  40:	441a      	add	r2, r3
  42:	6a3b      	ldr	r3, [r7, #32]
  44:	4413      	add	r3, r2
  46:	617b      	str	r3, [r7, #20]
  48:	697b      	ldr	r3, [r7, #20]
  4a:	4618      	mov	r0, r3
  4c:	371c      	adds	r7, #28
  4e:	46bd      	mov	sp, r7
  50:	f85d 7b04 	ldr.w	r7, [sp], #4
  54:	4770      	bx	lr
  56:	bf00      	nop

```
由上面反組譯碼可以看到,大抵上皆與第一個副程式相同惟獨第15行的"ldr r3, [r7, #32]"與前者不同,此處將指標指回呼叫副程式前存入的堆疊記憶體,並將其存入R3而非R4.由此可以了解到大於4個引數時副程式的執行會到需要第四個引數時才去存取堆疊來使用.
4.最後再用第三個副程式觀察如果使用指標進行傳遞程式會如何運行,先從主程式看起:
```
  cc:	f107 0014 	add.w	r0, r7, #20
  d0:	f107 0110 	add.w	r1, r7, #16
  d4:	f107 020c 	add.w	r2, r7, #12
  d8:	f107 0408 	add.w	r4, r7, #8
  dc:	1d3b      	adds	r3, r7, #4
  de:	9300      	str	r3, [sp, #0]
  e0:	4623      	mov	r3, r4
  e2:	f7ff ffb9 	bl	58 <MutiReturn>
  e6:	e7fe      	b.n	e6 <reset_handler+0x4e>   //while(1)
 ```
由上述反組譯碼可以清楚看到如同第二的副程式的結果在call function時程式會使用R1,R2,R3進行資料傳遞,超過的部分會放入堆疊記憶體做儲存,而且R3會優先用於資料運算,在第四行可以看到資料先被存放置R4,而後將R3推入堆疊後,再將R4移至R3,因此與第二個副程式相同,堆疊中存放的會是最後一個引數.以下再觀察副程式的處理狀況:
 ```
  58:	b480      	push	{r7}
  5a:	b085      	sub	sp, #20
  5c:	af00      	add	r7, sp, #0
  5e:	60f8      	str	r0, [r7, #12]
  60:	60b9      	str	r1, [r7, #8]
  62:	607a      	str	r2, [r7, #4]
  64:	603b      	str	r3, [r7, #0]
  66:	68fb      	ldr	r3, [r7, #12]
  68:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  6c:	60fb      	str	r3, [r7, #12]
  6e:	68bb      	ldr	r3, [r7, #8]
  70:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  74:	60bb      	str	r3, [r7, #8]
  76:	687b      	ldr	r3, [r7, #4]
  78:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  7c:	607b      	str	r3, [r7, #4]
  7e:	683b      	ldr	r3, [r7, #0]
  80:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  84:	603b      	str	r3, [r7, #0]
  86:	69bb      	ldr	r3, [r7, #24]
  88:	f503 73c8 	add.w	r3, r3, #400	; 0x190
  8c:	61bb      	str	r3, [r7, #24]
  8e:	3714      	adds	r7, #20
  90:	46bd      	mov	sp, r7
  92:	f85d 7b04 	ldr.w	r7, [sp], #4
  96:	4770      	bx	lr 
 ```
 可以由此看到與第二個副程式執行大同小異,再執行完R1到R3的運算後會借由R7指向Call function前放入堆疊的資料,另外因為借由指標做引數的副程式無須回傳所以此處並無將回傳值放入R0的程序.
## 3. 結果與討論
1.在AAPC架構中argument只能有三個置於暫存器,其餘皆存放於堆疊中.  
2.R3會被優先用於計算資料,R7優先用於存放SP與進行SP的移位.
3.若有回傳資料會置於R0中進行回傳

