all: ood draw

ood: ood.c
	gcc $< -o $@

draw: draw.c
	gcc $< -o $@

clean:
	@rm -f *.exe *.o
	@echo ...done.
