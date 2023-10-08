# compilerCodeGeneration
Program Translation Project. Program to parse the input, generate the parse tree, perform static semantics, and then generate a target file.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Any error should dislay detailed message, including line number if availabe (depending on scanner). Since p1-p3 were already tested, this focuses on testing p target code and storage allocation. Testing will be performed by running the generated targets to assess if the runtime behavior is correct. 

Invocation

compfs [file]    || ./compfs followed by keyboard input, then Ctl-D to produce target.
make clean to clean.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The target name should be kb.asm if keyboard input
file.asm if file was the input. So if the input is file.m then the output filename would be file.m.asm
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Storage allocation: Local - this must be matched with the local option in p3; variables considered global in p3 are scoped in the entire program (persistent) and variables in a block are scoped in the block as in p3.
Temporary variables can be processed as global scope even doing local scoping for user variables.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Upon success display only the name of the target file generated and no other output. Upon error, only display error message no other display should be generated and no target should be generated. 
