movies: movies.c movie.h names.c
	gcc movies.c names.c -o movies -Wall

clean: 
	rm -f *.o movies
