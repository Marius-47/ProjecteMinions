all: minions

obj/main.o: main.c user/user.h gru/gru.h
	gcc -g -c main.c -o obj/main.o

obj/user.o: user/user.c user/user.h
	gcc -g -c user/user.c -o obj/user.o

obj/utils.o: utils/utils.c utils/utils.h
	gcc -g -c utils/utils.c -o obj/utils.o

obj/tool.o: tool/tool.c tool/tool.h
	gcc -g -c tool/tool.c -o obj/tool.o

obj/task.o: task/task.c task/task.h
	gcc -g -c task/task.c -o obj/task.o

obj/gru.o: gru/gru.c gru/gru.h task/task.h user/user.h utils/utils.h
	gcc -g -c gru/gru.c -o obj/gru.o

minions: obj/main.o obj/user.o obj/tool.o obj/task.o obj/utils.o obj/gru.o
	gcc -g obj/main.o obj/user.o obj/tool.o obj/task.o obj/utils.o obj/gru.o -o minions

clean:
	rm -f obj/*.o minions