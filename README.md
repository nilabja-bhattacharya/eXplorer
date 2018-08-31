# OS_Project: eXplorer

There are two modes to use this application
1. Normal Mode
2. Command Mode

### **_NORMAL MODE_**
We can use Normal Mode to traverse directory
Keys to access directory are:
1. ENTER: Pressing ENTER will take us to the directory and open the file which the cursor is highlighting
2. BACKSPACE: Pressing BACKSPACE will take us to parent directory of the current directory
3. LEFT ARROW: Pressing LEFT ARROW will take us to previous directory we were in 
4. RIGHT ARROW: Pressing RIGHT ARROW will take us to the directory that we were in before pressing LEFT ARROW
5. COLON: Pressing COLON takes us to command mode
6. Q: Pressing Q exits the application

### **_COMMAND MODE_**
Various functions of command mode are:
1. **_copy_**: copy command will copy the file or directory to the destination directory </br>
         **syntax: copy <filename or directory name - 1> <filename or directory name - 2> <destination directory>** </br>
        where filename must be relative to path from which command mode was opened </br>
        If destination directory path is relative to path from which command mode was open it must be followed by a ~, i.e., ~/C/D otherwise the application expects absolute path.</br>

2. **_move_**: move command will move the file or directory to the destination directory</br>
         **syntax: move <filename or directory name - 1> <filename or directory name - 2> <destination directory>** </br>
        where filename must be relative to path from which command mode was opened</br>
        If destination directory path is relative to path from which command mode was opened it must be followed by a ~, i.e., ~/C/D otherwise the application expects absolute path.</br>
        
3. **_rename_**: rename command will rename the file and folder from old name to new name
           **syntax: rename <old name> <new name>**</br>
            The application expects the folder or file path provided is relative to path from which command mode was opened</br>

4. **_create_file_**: create_file command creates file in the destination directory</br>
            **syntax: create_file <filename 1 > <filename 2> <destination directory>**</br>
            The application expects the destination directory name provided is relative to the path from which command mode was opened, if it is of form, ~/C/D, or it is the root directory for application if destination directory is provided as '.'</br>

5. **_create_dir_**: create_dir command creates directory in the destination directory</br>
            **syntax: create_file <directory name 1 > <directory name 2> <destination directory>**</br>
            The application expects the destination directory path provided is relative to the path from which command mode was opened, ie, it is of form, ~/C/D, or it is the root directory for application if destination directory is provided as '.'</br>

6. **_delete_file_**: delete_file command deletes the file for which path was provided</br>
                **syntax: delete_file <filename 1> <filename 2> ..**</br>
                The application expects the file path to be relative to the path from which application was opened, if it is of form, ~/C/D, otherwise pathname is relative to path from which command mode was opened</br>

7. **_delete_dir_**: delete_dir command deletes the file for which path was provided</br>
                **syntax: delete_dir <directory name 1> <directory name 2> ..**</br>
                The application expects the directory path to be relative to the path from which application was opened, if it is of form, ~/C/D, otherwise pathname is relative to path from which command mode was opened</br>

8. **_goto_**: goto command takes us to the directory for which path was provided and the application switches          to normal mode. </br>
         **syntax: goto <directory path>**</br>
        The application expects the directory path to be relative to the path from which application was opened and to be root directory for application if '/' is provided as directory path</br>

9. **_search_**: search command searches files and directory with given name in current directory                         and recursively in all directory inside present directory</br>
           **syntax: search <filename or directory name>**</br>
            The application expects the directory path to be relative to the path from which command mode was opened</br>

10. **_snapshot_**: snapshot command takes snapshot of the directory and write it to the given file and it                   works like ls -R shell command, it also displays the result for 5 sec in the application                 window</br>
              **syntax: snapshot <directory name> <filename>**</br>
              The application expects the directory path and file path to be relative from which command mode was opened</br>
            
11. **_quit_**: quit command quits the application
          **syntax: quit**</br>