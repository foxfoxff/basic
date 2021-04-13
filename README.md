# BASIC
a minimal basic interpreter with two different versions implemented by C++ and QT GUI
## Discription
- You can edit code ended with a code line full with a linenumber and END in commandline window in the bottom of the mainwindow.

- Now it supports statements and commands listed below:

    INPUT,PRINT,LET,IF THEN,GOTO,REM,RUN,HELP,LOAD,END

- Mention that LET,INPUT and PRINT can be recognized without a line number,it will be excuted immediately, and  will not be displayed in the form of syntax tree.

- Another thing you need to remember is that any statement except command and three special statement listed above, will be ignored, which means it won't be displayed and won't have any effect to the program you typed in before.

- To optimize your coding experience, the interpreter implement  real-time sort by line number, so you don't need to type in LIST command.

- You can also load code file by type LOAD or click the button LOAD,it will open the file you choose and read the file line by line.

- Type RUN to make your simple Basic program run,it will excuted your code in the order of the line number.

- After you run the program, a series of syntax trees of the code you type in will be displayed in the syntax tree part.

- Type QUIT or simply click the close button to exit.

- To clear the window and variables you difined, you can just type CLEAR or click the button CLEAR.

- Statement with syntax error will also be reminder in the syntax tree display part,you will see an Error flowing the linenumber.

- Each time you type in code, the statement you type in should be case-sensitive.
- For more information or detail about the syntax of BASIC, read the file BASIC-doc.pdf.
## About the two versions
version-1 and version-2 have different execution logic. Version-1 is more like a real-time IDE,it will point out your syntax error and build a syntax tree each time you  type in code.
while version-2 will ignore them until you RUN your program, it will parse statements after RUN command, and then build syntax trees, which include syntax errors in your program.
