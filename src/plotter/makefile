SKETCH = plotter.ino
BOARD = arduino:avr:mega
PORT = /dev/ttyACM0   
CLI = arduino-cli

.PHONY: all build upload clean monitor

all: build

build:
	$(CLI) compile --fqbn $(BOARD) $(SKETCH)

upload:
	$(CLI) upload -p $(PORT) --fqbn $(BOARD) $(SKETCH)

clean:
	$(CLI) cache clean
	rm -rf build/

monitor:
	$(CLI) monitor -p $(PORT)