.PHONY: all prepare clean install uninstall

PREFIX_DIR = /home/rouvik/Programming/PackageManager/.ropm/bin

BUILD_DIR = ./bin

PACKAGE_NAME=myprog

all: prepare
	$(CC) main.c -o $(BUILD_DIR)/$(PACKAGE_NAME)

prepare:
	mkdir -p $(BUILD_DIR)

install:
	cp -r $(BUILD_DIR) $(PREFIX_DIR)/$(PACKAGE_NAME).build
	cp Makefile $(PREFIX_DIR)/$(PACKAGE_NAME).build
	ln $(PREFIX_DIR)/$(PACKAGE_NAME).build/$(PACKAGE_NAME) $(PREFIX_DIR)

uninstall:
	rm -r $(PREFIX_DIR)/$(PACKAGE_NAME).build
	rm $(PREFIX_DIR)/$(PACKAGE_NAME)

clean:
	rm -r $(BUILD_DIR)