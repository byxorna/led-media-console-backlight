.DEFAULT_GOAL := build
FIRMWARE = firmware.bin
TARGET = latest
deps:
	@echo Installing deps
	yarn global add particle-cli

.PHONY: $(FIRMWARE)
$(FIRMWARE):
	particle compile photon . --target $(TARGET) --saveTo $(FIRMWARE)

.PHONY: build
build: $(FIRMWARE)

.PHONY: flash
flash: build $(FIRMWARE)
	@echo Flashing firmware
	particle flash --usb $(FIRMWARE)
