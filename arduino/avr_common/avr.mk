# set this to false to disable sonars in firmware
CC=avr-gcc
AS=avr-gcc
INCLUDE_DIRS=-I. -I../avr_common
CC_OPTS=-Wall --std=gnu99 -DF_CPU=16000000UL -O3 -funsigned-char -funsigned-bitfields  -fshort-enums -Wall -Wstrict-prototypes -mmcu=atmega2560 $(INCLUDE_DIRS)  -D__AVR_3_BYTE_PC__
AS_OPTS=-x assembler-with-cpp $(CC_OPTS)

AVRDUDE=avrdude

# com1 = serial port. Use lpt1 to connect to parallel port.
AVRDUDE_PORT = /dev/tty.usbserial-110    # programmer connected to serial device

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET):i
AVRDUDE_FLAGS = -b 115200 -p m2560 -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)	
AVRDUDE_FLAGS += -q -V -C /usr/local/etc/avrdude.conf
AVRDUDE_FLAGS += -c wiring -D 

.phony:	clean all

all:	$(BINS) 

#common objects
%.o:	%.c 
	$(CC) $(CC_OPTS) -c  -o $@ $<

%.o:	%.s 
	$(AS) $(AS_OPTS) -c  -o $@ $<

%.elf:	%.o $(OBJS)
	$(CC) $(CC_OPTS) -o $@ $< $(OBJS) $(LIBS)


%.hex:	%.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$@:i #$(AVRDUDE_WRITE_EEPROM) 

clean:	
	rm -rf $(OBJS) $(BINS) *.hex *~ *.o

.SECONDARY:	$(OBJS)
