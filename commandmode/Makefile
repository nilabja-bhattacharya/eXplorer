CC = g++

CFLAGS = -c -Wall

all: commandmode

commandmode: copy_block.o make_directory.o create_file.o create_directory.o copy_files_inside_directory.o copy_directory.o delete_directory.o delete_file.o goto_directory.o list_directory.o make_source_directory.o move_directory.o move_file.o rename_file.o search_file_or_directory.o snapshot_directory.o trim_pathname.o 
			$(CC) copy_block.o create_file.o make_directory.o create_directory.o copy_files_inside_directory.o copy_directory.o delete_directory.o delete_file.o goto_directory.o list_directory.o make_source_directory.o move_directory.o move_file.o rename_file.o search_file_or_directory.o snapshot_directory.o trim_pathname.o

copy_block.o: copy_block.cpp
		$(CC) $(CFLAGS) copy_block.cpp

create_file.o: create_file.cpp
		$(CC) $(CFLAGS) create_file.cpp

create_directory.o: create_directory.cpp
		$(CC) $(CFLAGS) create_directory.cpp

copy_files_inside_directory.o: copy_files_inside_directory.cpp
		$(CC) $(CFLAGS) copy_files_inside_directory.cpp
		
copy_directory.o: copy_directory.cpp
		$(CC) $(CFLAGS) copy_directory.cpp

delete_directory.o: delete_directory.cpp
		$(CC) $(CFLAGS) delete_directory.cpp

delete_file.o: delete_file.cpp
		$(CC) $(CFLAGS) delete_file.cpp

goto_directory.o: goto_directory.cpp
		$(CC) $(CFLAGS) goto_directory.cpp

list_directory.o: list_directory.cpp
		$(CC) $(CFLAGS) list_directory.cpp

make_directory.o: make_directory.cpp
		$(CC) $(CFLAGS) make_directory.cpp

make_source_directory.o: make_source_directory.cpp
		$(CC) $(CFLAGS) make_source_directory.cpp

move_directory.o: move_directory.cpp
		$(CC) $(CFLAGS) move_directory.cpp

move_file.o: move_file.cpp
		$(CC) $(CFLAGS) move_file.cpp

rename_file.o: rename_file.cpp
		$(CC) $(CFLAGS) rename_file.cpp

search_file_or_directory.o: search_file_or_directory.cpp
		$(CC) $(CFLAGS) search_file_or_directory.cpp

snapshot_directory.o: snapshot_directory.cpp
		$(CC) $(CFLAGS) snapshot_directory.cpp

trim_pathname.o: trim_pathname.cpp
		$(CC) $(CFLAGS) trim_pathname.cpp

clean: 
		rm -rf *o commandmode