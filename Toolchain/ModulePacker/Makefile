MP=mp.bin
SOURCES=$(wildcard *.c)

all: $(MP)
	@echo -e "ModulePacker		[ \033[0;32mOK \033[0m]"

$(MP): $(SOURCES)
	@gcc $(SOURCES) -o $(MP)

clean:
	@rm -rf $(MP)

.PHONY: all clean