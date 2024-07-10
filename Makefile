.PHONY: build, deploy, clean

build: src/main.c src/ir_driver.c
	avr-gcc -O -mmcu=atmega328p $^ -o main
	avr-objcopy -O ihex -R .eeprom main main.hex

deploy: build
	avrdude -F -V -c arduino -p m328p -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex

clean:
	rm -f main main.hex
