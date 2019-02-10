all:
	#compila
	gcc -c bitmap.c
	gcc -c TadArvore.c
	gcc -c TadLista.c
	gcc -c compacta.c
	gcc -c descompacta.c
	#gera executável
	gcc TadArvore.o TadLista.o compacta.o bitmap.o -o compacta
	gcc TadArvore.o TadLista.o descompacta.o bitmap.o -o descompacta -lm
	
clean:
	rm -rf *.o *.comp compacta descompacta
	
compactados:
	#apaga arquivos compactados
	rm -f *.comp
