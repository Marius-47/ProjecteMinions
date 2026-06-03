all: minions

obj/main.o: main.c user/user.h
	gcc -c main.c -o obj/main.o

obj/user.o: user/user.c user/user.h
	gcc -c user/user.c -o obj/user.o

obj/utils.o: utils/utils.c utils/utils.h
	gcc -c utils/utils.c -o obj/utils.o

minions: obj/main.o obj/user.o obj/utils.o
	gcc obj/main.o obj/user.o obj/utils.o -o minions

clean:
	rm -f obj/*.o minions