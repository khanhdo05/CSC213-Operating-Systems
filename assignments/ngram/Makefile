CC := clang
CFLAGS := -g -Wall -Werror

all: ngram

clean:
	@echo "Removing build output"
	@rm -f ngram ngram.zip

ngram: ngram.c
	$(CC) $(CFLAGS) -o ngram ngram.c

zip:
	@echo "Generating ngram.zip file to submit to Gradescope..."
	@zip -q -r ngram.zip . -x .git/\* .vscode/\* .clang-format .gitignore ngram
	@echo "Done. Please upload ngram.zip to Gradescope."

format:
	@echo "Reformatting source code."
	@clang-format -i --style=file $(wildcard *.c) $(wildcard *.h)
	@echo "Done."

.PHONY: all clean zip format
