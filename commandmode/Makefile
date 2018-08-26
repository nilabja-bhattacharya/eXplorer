CC = gcc

CFLAGS = -c -Wall

all: commandmode

commandmode: test.o copy_block.o make_directory.o create_file.o create_directory.o copy_files_inside_directory.o copy_directory.o delete_directory.o delete_file.o goto_directory.o list_directory.o make_source_directory.o move_directory.o move_file.o rename_file.o search_file_or_directory.o snapshot_directory.o trim_pathname.o 
			$(CC) test.o copy_block.o create_file.o make_directory.o create_directory.o copy_files_inside_directory.o copy_directory.o delete_directory.o delete_file.o goto_directory.o list_directory.o make_source_directory.o move_directory.o move_file.o rename_file.o search_file_or_directory.o snapshot_directory.o trim_pathname.o -o commandmode

test.o: test.c
		$(CC) $(CFLAGS) test.c

copy_block.o: copy_block.c
		$(CC) $(CFLAGS) copy_block.c

create_file.o: create_file.c
		$(CC) $(CFLAGS) create_file.c

create_directory.o: create_directory.c
		$(CC) $(CFLAGS) create_directory.c

copy_files_inside_directory.o: copy_files_inside_directory.c
		$(CC) $(CFLAGS) copy_files_inside_directory.c
		
copy_directory.o: copy_directory.c
		$(CC) $(CFLAGS) copy_directory.c

delete_directory.o: delete_directory.c
		$(CC) $(CFLAGS) delete_directory.c

delete_file.o: delete_file.c 
		$(CC) $(CFLAGS) delete_file.c

goto_directory.o: goto_directory.c
		$(CC) $(CFLAGS) goto_directory.c

list_directory.o: list_directory.c
		$(CC) $(CFLAGS) list_directory.c

make_directory.o: make_directory.c
		$(CC) $(CFLAGS) make_directory.c

make_source_directory.o: make_source_directory.c
		$(CC) $(CFLAGS) make_source_directory.c

move_directory.o: move_directory.c
		$(CC) $(CFLAGS) move_directory.c

move_file.o: move_file.c
		$(CC) $(CFLAGS) move_file.c

rename_file.o: rename_file.c
		$(CC) $(CFLAGS) rename_file.c

search_file_or_directory.o: search_file_or_directory.c
		$(CC) $(CFLAGS) search_file_or_directory.c

snapshot_directory.o: snapshot_directory.c
		$(CC) $(CFLAGS) snapshot_directory.c

trim_pathname.o: trim_pathname.c
		$(CC) $(CFLAGS) trim_pathname.c

clean: 
		rm -rf *o commandmode