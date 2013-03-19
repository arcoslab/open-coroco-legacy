CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atmega128
LFLAGS=-lm
OBJ2HEX=avr-objcopy 
UISP=uisp 
TARGET=pwm
SOURCES=Hardware.c UART.c Timer.c

program : $(TARGET).hex
#echo $(TARGET).hex ready to be programmed
#$(UISP) -dprog=stk500 -dserial=/dev/ttyS1 --erase -dpart=atmega32
#$(UISP) -dprog=stk500 -dserial=/dev/ttyS1 --upload -dpart=atmega32 \
#	if=$(TARGET).hex -v=2
$(TARGET).obj : main.c $(SOURCES:.c=.o)
	echo $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) main.c -o $@ $(SOURCES:.c=.o) $(LFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $?

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $? $@

#%.bin : %.obj
#	$(OBJ2HEX) -R .eeprom -O binary $< $@
#%.elf : %.obj
#	$(OBJ2HEX) -R .eeprom -O elf32-avr $< $@

clean :
	rm -f *.hex *.obj *.o

