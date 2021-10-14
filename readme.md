# How to Compile/Build the project in VS Code (or any terminal)

- Use the command below to rcompile the code

		g++ -o test.exe board.cpp board.h cell.cpp cell.h

- To run the created text.exe file use the command below
	
		./test	
**NB: the forward forward-slash works only in bash terminal, if you're using windows terminal/powershell please use the back-slash**

- To run the created **test.exe** file parsing command line arguments to it use the command below <br>

		./test moves.txt
**NB: You can pass more than one command line argument make sure separate them with a space. **