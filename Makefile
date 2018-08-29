CC = g++

CFLAGS = -c -Wall

all: eXplorer

eXplorer: eXplorer.o copy_block.o make_directory.o create_file.o create_directory.o copy_files_inside_directory.o copy_directory.o delete_directory.o delete_file.o goto_directory.o list_directory.o make_source_directory.o move_directory.o move_file.o rename_file.o search_file_or_directory.o snapshot_directory.o trim_pathname.o 
			$(CC) eXplorer.o copy_block.o create_file.o make_directory.o create_directory.o copy_files_inside_directory.o copy_directory.o delete_directory.o delete_file.o goto_directory.o list_directory.o make_source_directory.o move_directory.o move_file.o rename_file.o search_file_or_directory.o snapshot_directory.o trim_pathname.o -o eXplorer

eXplorer.o: eXplorer.cpp
		$(CC) $(CFLAGS) eXplorer.cpp

copy_block.o: commandmode/copy_block.cpp
		$(CC) $(CFLAGS) commandmode/copy_block.cpp

create_file.o: commandmode/create_file.cpp
		$(CC) $(CFLAGS) commandmode/create_file.cpp

create_directory.o: commandmode/create_directory.cpp
		$(CC) $(CFLAGS) commandmode/create_directory.cpp

copy_files_inside_directory.o: commandmode/copy_files_inside_directory.cpp
		$(CC) $(CFLAGS) commandmode/copy_files_inside_directory.cpp
		
copy_directory.o: commandmode/copy_directory.cpp
		$(CC) $(CFLAGS) commandmode/copy_directory.cpp

delete_directory.o: commandmode/delete_directory.cpp
		$(CC) $(CFLAGS) commandmode/delete_directory.cpp

delete_file.o: commandmode/delete_file.cpp
		$(CC) $(CFLAGS) commandmode/delete_file.cpp

goto_directory.o: commandmode/goto_directory.cpp
		$(CC) $(CFLAGS) commandmode/goto_directory.cpp

list_directory.o: commandmode/list_directory.cpp
		$(CC) $(CFLAGS) commandmode/list_directory.cpp

make_directory.o: commandmode/make_directory.cpp
		$(CC) $(CFLAGS) commandmode/make_directory.cpp

make_source_directory.o: commandmode/make_source_directory.cpp
		$(CC) $(CFLAGS) commandmode/make_source_directory.cpp

move_directory.o: commandmode/move_directory.cpp
		$(CC) $(CFLAGS) commandmode/move_directory.cpp

move_file.o: commandmode/move_file.cpp
		$(CC) $(CFLAGS) commandmode/move_file.cpp

rename_file.o: commandmode/rename_file.cpp
		$(CC) $(CFLAGS) commandmode/rename_file.cpp

search_file_or_directory.o: commandmode/search_file_or_directory.cpp
		$(CC) $(CFLAGS) commandmode/search_file_or_directory.cpp

snapshot_directory.o: commandmode/snapshot_directory.cpp
		$(CC) $(CFLAGS) commandmode/snapshot_directory.cpp

trim_pathname.o: commandmode/trim_pathname.cpp
		$(CC) $(CFLAGS) commandmode/trim_pathname.cpp

clean: 
		rm -rf *o commandmode