# OS_Project: eXplorer

There are two modes to use this application
1. Normal Mode
2. Command Mode

### **_NORMAL MODE_**
We can use Normal Mode to traverse directory

#### Keys to access directory are:
- ENTER: ENTER can be used to enter directory or open files and application
- BACKSPACE: Pressing BACKSPACE will take us to the parent directory of the current directory
- LEFT ARROW: Pressing LEFT ARROW will take us to previous directory that we were in 
- RIGHT ARROW: Pressing RIGHT ARROW will take us to the directory that we were in before pressing the LEFT ARROW
- COLON: Pressing COLON takes us to command mode
- Q: Pressing Q exits the application



### **_COMMAND MODE_**

#### Various functions of command mode are:
- **_copy_**: copy command will copy the file or directory to the destination directory

        syntax: copy < filename or directory name - 1 > < filename or directory name - 2 > < destination directory >
        
        where filename must be relative to path  to current directory, if the path isn't preceded by '/' or '~'
        If destination directory path is relative to root directory for application it must be followed by a '~' or '/' i.e., ~/C/D or /C/D otherwise the application expects path is relative to current directory

- **_move_**: move command will move the file or directory to the destination directory

        syntax: move < filename or directory name - 1 > < filename or directory name - 2 > < destination directory >
       
        where filename must be relative to path  to current directory, if the path isn't preceded by '/' or '~'
        If destination directory path is relative to root directory for application it must be followed by a '~' or '/' i.e., ~/C/D or /C/D otherwise the application expects path is relative to current directory

        
- **_rename_**: rename command will rename the file and folder from old name to new name

           syntax: rename < old name > < new name >
           
           The application expects the folder or file path provided is relative to current directory if the filepath isn't preceded by '/' otherwise, the application expects the filepath is relative to root directory of application

- **_create_file_**: create_file command creates file in the destination directory

            syntax: create_file < filename 1 > < filename 2 > < destination directory >
            
            The application expects the destination directory name provided is relative to root directory of application, if it is of form, ~/C/D or /C/D, or it is the current directory if destination directory is provided as '.' or it is relative to current diectory if it is of form C/D

- **_create_dir_**: create_dir command creates directory in the destination directory

            syntax: create_file < directory name 1 > < directory name 2 > < destination directory >
            
            The application expects the destination directory name provided is relative to root directory of application, if it is of form, ~/C/D or /C/D, or it is the current directory if destination directory is provided as '.' or it is relative to current diectory if it is of form C/D

- **_delete_file_**: delete_file command deletes the file for which path was provided

                syntax: delete_file < filename 1 > < filename 2 > ..
                
                The application expects the file path to be relative to the root directory of application, if it is of form, ~/C/D or /C/D, otherwise pathname is relative to current directory

- **_delete_dir_**: delete_dir command deletes the file for which path was provided

                syntax: delete_dir < directory name 1 > < directory name 2 > ..
                
                The application expects the file path to be relative to the root directory of application, if it is of form, ~/C/D or /C/D, otherwise pathname is relative to current directory
                
- **_goto_**: goto command takes us to the directory for which path was provided and the application switches to normal mode

        syntax: goto < directory path >
        
        The application expects the directory path to be root directory if directory path is provides as '/'. If directory path is provided as ~/C/D or /C/D then application expects it to be relative to root directory, if directory path is provided as C/D, then application expects it to be relative to current directory

- **_search_**: search command searches files and directory with given name in current directory and recursively in all directory inside current directory, it displays the result in normal mode.

           syntax: search < filename or directory name >

- **_snapshot_**: snapshot command takes snapshot of the directory and write it to the given file, it also displays the result for 5 sec in the application window (it works works like ls -R shell command)

              syntax: snapshot < directory name > < filename >
              
              The application expects the directory path and file path to be relative to current directory if path is of form C/D or relative to root directory of appication if it is of form /C/D
            
- **_quit_**: quit command quits the application

          syntax: quit
