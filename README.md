# OS_Project: eXplorer

There are two modes to use this application
1. Normal Mode
2. Command Mode

## NORMAL MODE
We can use Normal Mode to traverse directory
Keys to access directory are:
ENTER: Pressing ENTER will take us to the directory and open the file which the cursor is highlighting
BACKSPACE: Pressing BACKSPACE will take us to parent directory of the current directory
LEFT ARROW: Pressing LEFT ARROW will take us to previous directory we were in 
RIGHT ARROW: Pressing RIGHT ARROW will take us to the directory that we were in before pressing LEFT ARROW
COLON: Pressing COLON takes us to command mode
Q: Pressing Q exits the application

## COMMAND MODE
Various functions of command mode are:
1. copy: copy command will copy the file or directory to the destination directory
         <syntax>: copy <filename or directory name - 1> <filename or directory name - 2> <destination directory> 
        where filename must be relative to path from which command mode was opened
        If destination directory path is relative to path from which command mode was open it must be followed by a ~, i.e., ~/C/D otherwise the application expects absolute path.

2. move: move command will move the file or directory to the destination directory
         <syntax>: move <filename or directory name - 1> <filename or directory name - 2> <destination directory> 
        where filename must be relative to path from which command mode was opened
        If destination directory path is relative to path from which command mode was opened it must be followed by a ~, i.e., ~/C/D otherwise the application expects absolute path.
        
3. rename: rename command will rename the file and folder from old name to new name
           <syntax>: rename <old name> <new name>
            The application expects the folder or file path provided is relative to path from which command mode was opened

4. create_file: create_file command creates file in the destination directory
            <syntax>: create_file <filename 1 > <filename 2> <destination directory>
            The application expects the destination directory name provided is relative to the path from which command mode was opened, if it is of form, ~/C/D, or it is the root directory for application if destination directory is provided as '.'

5. create_dir: create_dir command creates directory in the destination directory
            <syntax>: create_file <directory name 1 > <directory name 2> <destination directory>
            The application expects the destination directory path provided is relative to the path from which command mode was opened, ie, it is of form, ~/C/D, or it is the root directory for application if destination directory is provided as '.'

6. delete_file: delete_file command deletes the file for which path was provided
                <syntax>: delete_file <filename 1> <filename 2> ..
                The application expects the file path to be relative to the path from which application was opened, if it is of form, ~/C/D, otherwise pathname is relative to path from which command mode was opened

7. delete_dir: delete_dir command deletes the file for which path was provided
                <syntax>: delete_dir <directory name 1> <directory name 2> ..
                The application expects the directory path to be relative to the path from which application was opened, if it is of form, ~/C/D, otherwise pathname is relative to path from which command mode was opened

8. goto: goto command takes us to the directory for which path was provided and the application switches          to normal mode.
         <syntax>: goto <directory path>
        The application expects the directory path to be relative to the path from which application was opened and to be root directory for application if '/' is provided as directory path

9. search: search command searches files and directory with given name in current directory                         and recursively in all directory inside present directory
           <syntax>: search <filename or directory name>
            The application expects the directory path to be relative to the path from which command mode was opened

10. snapshot: snapshot command takes snapshot of the directory and write it to the given file and it                   works like ls -R shell command, it also displays the result for 5 sec in the application                 window
              <syntax>: snapshot <directory name> <filename>
              The application expects the directory path and file path to be relative from which command mode was opened
            
11. quit: quit command quits the application
          <syntax>: quit