CFLAGS=-O2 -Wall -Wextra
FILES=main.c complex_mul.c btoc.c ctob.c utils.c test.c base_powers.c performance.c

.PHONY: clean
all: cbns

cbns: $(FILES)
	$(CC) $(CFLAGS) -o $@ $^
	 
clean:
	rm -rf cbns