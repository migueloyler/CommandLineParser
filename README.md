

The shell is the program that reads and interprets the commands we type in a command-line environment. Here we will implement one component of a shell: the command parser. The parser takes a command line comprised of a program name and its command-line arguments (e.g., 'gcc -Wall file1.c file2.c') and converts it to a command array, which is an array of the strings comprising the command line (e.g., ['gcc', '-Wall', 'file1.c', 'file2.c']). This process is similar to the behavior of the split function available in many programming languages. 

This proect consists of the following files:

command.c: a skeleton for the command parser, and the primary file in which we will write our code. Note that this file does not (and should not) contain a main function, as it is simply a collection of library functions designed to be called elsewhere.
command.h: a C 'header file' declaring the functions implemented in command.c.
command_test.c: test code for the command parser (which we will extend). This file contains our main function, which will invoke the various functions defined in command.c to test their functionality.
Makefile: used to build and test the parser.
To summarizing the above, since our command parser is essentially a software library (that is, it provides functionality for other programs rather than being a standlone executable itself), our command.c file does not have a main function and cannot be executed directly. Instead, the actual executable we will run is derived from command_test.c. Running make will build the test executable, which can then be executed with ./command_test.

Our primary job will be to complete the parsing functions in command.c. We will also write additional tests within command_test.c to exercise the parser.

Command Line Structure
A command line is defined to be a null-terminated string containing zero or more words separated by one or more spaces, with an optional ampersand ('&') after the final word. This ampersand is used to denote a background command. In particular:

Valid characters within a word include all printable characters except spaces and ampersands.
Ampersand ('&') is a special status indicator, not a regular word character.
In order for a command line to be valid, an ampersand may appear at most once, may only appear after the final word, and may be followed by zero or more spaces (and nothing else) before reaching the end of the line. The ampersand may be separated from the final word by zero or more spaces.
A command line including an ampersand as described above is a background command, while a command line without an ampersand is a foreground command.
Note that the rules above are slightly more restrictive than in a real shell program (just for simplicity's sake).

Command Line Examples
Here are two typical command lines:

The line "ls -l fcs-labs" indicates a foreground command containing the words 'ls', '-l', and 'fcs-labs'.
The line "nano proj2/command.c &" indicates a background command containing the words 'nano' and 'proj2/command.c'.
Since the definitions allow for any spacing between words, the following lines are all allowed variations on the first example above:

"      ls -l fcs-labs"
"ls -l   fcs-labs      "
"  ls       -l   fcs-labs    "
Similarly, the following are all valid command lines:

"nano &"
"nano&"
"   nano&  "
However, the following are all examples of invalid command lines:

"&uhoh"
"  &   uh oh"
"uh & oh"
"uh oh & &"
Command Arrays
The parser converts a command line into a command array (an ordered array of strings representing the words of the command line) terminated by a NULL address. Recall that a string in C is not a special type; it is just an array of char terminated by a null character ('\0') -- the character with ASCII code zero. A command array is thus an array of pointers to arrays of characters, and therefore has the type char**. All arrays in this structure must be null-terminated. Note that there are two different notions of null here -- the null character '\0' used to terminate a string, and the special value NULL representing a null pointer and used to terminate the words array.

warningIMPORTANT: The null character ('\0') is not the same as the null address (NULL). The former is a character, and is therefore one byte in size. The latter is an address (i.e., pointer), and is therefore one machine word in size. Both have a numeric value of zero.

Also remember the difference between literal characters and literal strings. Characters in C are specified in single quotes ('a') and have type char. Literal strings are specified in double quotes ("a") and have type char*. As such, 'a' is not the same as "a". The former is a one-byte char with ASCII value 97, while the latter is a pointer to the start of a one-character, null-terminated char array (of length 2).

Here is an example command array for the command line string "ls -l fcs-labs":

Command Array:             Null-Terminated Strings
Index   Contents           (stored elsewhere in memory)
      +----------+
    0 |  ptr  *----------> "ls"
      +----------+
    1 |  ptr  *----------> "-l"
      +----------+
    2 |  ptr  *----------> "fcs-labs"
      +----------+
    3 |   NULL   |
      +----------+
Here is the same array drawn another way and showing how the array is arranged in memory, with each element's offset from the base address of the array. Addresses grow left to right, and are assumed to be 64 bits (8 bytes), so indices are related to offsets by a factor of 8.

Index:        0       1       2       3 
Offset:   +0      +8      +16     +24     +32
          +-------+-------+-------+-------+
Contents: |   *   |   *   |   *   | NULL  |
          +---|---+---|---+---|---+-------+
              |       |       |
              V       V       V
              "ls"    "-l"    "fcs-labs"
Note that although we draw "strings" in the above pictures, this is an abstraction. Each string is actually represented by a '\0'-terminated array of 1-byte characters in memory, as shown below for the first word. Since each element is one byte, the addresses of adjacent characters differ by 1 and the offset is identical to the index.

Index:      0     1     2
Offset:  +0    +1    +2    +3
         +-----+-----+-----+
         | 'l' | 's' |'\0' |
         +-----+-----+-----+
warningIMPORTANT: While the null characters at the end of strings and the NULL pointers at the end of command arrays are fulfilling the same basic purpose (indicating the end of the array), they exist for different reasons. Strings in C are always null-terminated, but regular (non-string) arrays are *not*. The only reason command arrays (which are not strings) have NULL pointers on the end is because that's the way we've defined them.

Function Specifications
We must write four functions in command.c (plus any necessary helper functions) supporting command parsing according to the headers in command.h. Summaries of each function are provided below:

char** command_parse(char* line, int* foreground): Parses a command-line string line and returns a command array containing the words of the command line. If the command line is not valid, returns NULL. Additionally, the function stores a value at the address foreground according to whether the command line is a foreground or background command. If foreground, the value stored should be 1 (i.e., true). If background, the value stored should be 0 (i.e., false). If the command line is not valid, nothing should be stored.
void command_print(char** command): Prints a command array in the form of a command line, with the command words separated by single spaces. Do not include quotes, do not include the '&' (for background commands) and do not include a newline ('\n') at the end. We should use the printf function here.
void command_show(char** command): Prints the structure of a command array to aid in debugging and data inspection. The output should make it clear what strings the command array holds, but the exact format is left to us. For example, we might choose to print each word of the command array on separate lines to clearly delineate the word boundaries.
void command_free(char** command): Frees (i.e., deallocates) all parts of a command array previously created by command_parse and not yet freed.


Coding Rules and C
This lab may be the first time you have coded a real program in C. Syntactically, C is very similar to Java, so you should have minimal difficulty with the basic building blocks (functions, conditionals, loops, etc). Conceptually, the primary challenge of this lab is grappling with pointers.

For general advice about proper program design and style, consult the CSCI 2330 Coding Design & Style Guide. You should review this guide before beginning to code and consult it as a general reference.

In addition to following good design and style principles, you are required to follow the following coding rules for this assignment:

You may not use any standard C string processing functions. Using printf is fine, but anything declared in string.h is not. Instead of using such functions, you must do all your string processing yourself (from scratch).
You may not use any array notation in command.c (but it is fine to do so in command_test.c). Instead, you should use idiomatic pointer style for working with your strings, which is described below. Practically speaking, this restriction is just a notational difference, but will force you to think about your structures explicitly in terms of pointers. More details are provided in the next section.
Your code must follow standard memory safety rules:

No use of uninitialized memory contents.
No out-of-bounds accesses.
free only called on blocks previously returned by malloc, and only once per block.
Note that violating one of these rules does not necessarily mean that your program will crash (e.g., if you access uninitialized memory), so the absence of crashes does not necessarily mean that you are following all the rules. Use the valgrind tool to check for many kinds of memory errors.

You should abide by these usage conventions within your library:

Clients of the command library (such as command_test.c) own and manage the memory representing command line strings. This means that command library functions must never free nor mutate command line strings (i.e., don't change any string that's passed to one of your functions), with the exception of command_free.
Command array structures must be allocated dynamically using malloc within command_parse and returned to the client. Clients will not mutate command array structures once they are returned.
Your library functions should not allocate any memory except that which is returned as part of the command array structure.
You may assume that clients will call command_free at most once on a given command array structure.
Assuming that command_free is eventually called on every command array structure, all memory allocated by the command library functions should be freed. In other words, your code should not leak memory.
Don't make any assumptions about data type sizes in your program. For example, remember that while an int is usually 32 bits, this isn't required by C and may vary across machines. Instead, use the C sizeof operator whenever you need the number of bytes in a particular data type. For instance, you should write sizeof(int) instead of 4, and sizeof(char) instead of 1, and so forth.
Lastly, treat compiler warnings like errors. Your code should not output any warnings when compiled, even if these warnings do not prevent the program from working.
Idiomatic Pointer Style
Your code must use only pointers and pointer arithmetic, with no array notation. In general, choosing pointer arithmetic over array indexing is not always the best choice for clear code, but here it will teach you about how arrays work at a lower level.

A simple way to think with arrays but write with pointers is to use *(a + i) wherever you think a[i]. However, this simple transformation will generally not produce an idiomatic pointer style.

A typical array loop with array indexing normally uses an integer index variable incremented on each iteration, such as in the following:

    // replaces all characters in string a by 'Z'
    for (int i = 0; a[i] != '\0'; i++) {
        a[i] = 'Z';
    }
While you could rewrite the above without array indexing by just directly applying the pointer substitution mentioned previously, a more idiomatic style uses a cursor pointer that is incremented to point to the next element on each iteration, as in the following:

    // replaces all characters in a string by 'Z'
    for (char* p = a; *p != '\0'; p++) {
        *p = 'Z';
    }
Importantly, note that the loop variable in the array style is an integer index, while the loop variable in the idiomatic pointer style is a character pointer (not an int).

You can simplify the above code even further by noting that '\0' has a numeric value of zero:

    // replaces all characters in a string by 'Z'
    for (char* p = a; *p; p++) {
        *p = 'Z';
    }
To reiterate the prohibition on array indexing: your final command.c code should contain zero array[index] operations, and probably no operations of the form *(p + i) for some pointer p and index i.

Suggested Workflow
Since there are several components of the library, here is a suggested plan of action for tackling them:

Add several more hard-coded command array test cases to command_test.c These are the lines of the form:
static char* NAME[N] = {"str1", "str2", ..., "strN"}
Two example command arrays are already included in command_test.c. The purpose of writing hard-coded command arrays here is that it lets you test the rest of the program minus the string parsing component (the command_parse function).
Within command.c, implement and test command_show and command_print. These functions should only be a few lines of code each. Test them on the constant, statically allocated command arrays in command_test.c.
Add several more hard-coded valid and invalid command lines to command_test.c to test all aspects of the specification.
Within command.c, implement and test command_parse in stages, testing each stage on several inputs and committing a working version before continuing:

Count the number of words in line and detect use of &, returning NULL for invalid commands and marking the foreground/background status for valid commands.
Allocate the top-level command array.
For each each word in line, allocate properly sized space to hold the word as a null-terminated string, copy the word into this space, and save it in the command array.
Implement and test command_free.
Rigorously test your library by adding many more test strings to the command lines in command_test.c. Test every aspect of the specification (e.g., valid lines, invalid lines, spacing, ampersands, function return values, etc) and make sure the behavior of your program matches the specification exactly. My own test suite that will be used to exercise your library has over 100 command lines in it -- while you may not need that many, one or two command lines will definitely not be sufficient to be confident that your program is correct.
Tools and Debugging Tips
Programming in C can be finicky and error-prone, even for experts. Make use of the tools available to aid in debugging whenever possible:

Make liberal use of assertions. Assertions are "executable documentation": they document rules about how code should be used or how data should be structured, but they also make it easier to detect violations of these rules (a.k.a. bugs!). Use the assert(...) statment in C by including assert.h and asserting expected properties. For example, the provided code already includes code that asserts that the arguments to command_ functions are not NULL. Thus, if a NULL argument is ever passed to these functions, an error message will be printed and execution will halt immediately. Detecting errors early like this (vs. crashing or corrupting data later wherever the code depends on this assumption) saves a lot of time. Add assertions to make the "rules" of your code clear wherever you make assumptions.

Your primary runtime debugging tools should be Valgrind and GDB. Completing the Debugging Mini-Lab is a good way to get up to speed on using these tools. Periodically running Valgrind when developing is a good idea to catch memory errors as early as possible. Refer to the GDB Reference Sheet when debugging in GDB.

Minimize print-based debugging (e.g., printf) in favor of the other tools mentioned above. If you do use printf, remember that you need to explicitly include ending newline characters (unlike, for example, System.out.println in Java or print in Python). However, be sure to disable all extraneous print commands in command.c in your final submitted version. Your program should not produce any output that the specification does not include!

C Function Declarations and Header Files
In C, a function is allowed to be used only after (that is, later in the file than) its declaration. This behavior differs from Java, which allows you to refer to later methods from earlier methods. When declaring helper functions, you can do one of a few things to deal with this restriction:

Just declare your helper function before the functions that use it.
Write a function header earlier in the file, then the actual definition later in the file. The function header just describes the name and type of the function, much like an interface method in Java. For example:
        // A function header declares that such a function exists,
        // and will be implemented elsewhere.
        int helper(int x, int y);

        // Parameter names are optional in headers.
        int helper2(char*);

        void needsHelp() {
            // OK, because header precedes this point in the file
            helper(7, 8);
            helper2("hello");
        }

        // even though the implementation comes later
        int helper(int x, int y) {
            return x + y;
        }

        int helper2(char* str) {
            return 7;
        }
If the functions would likely get used elsewhere, then put the header in a header file, which is a file ending in '.h' that contains only function headers (for related functions) and data type declarations. For example, if you added another general function (not just a helper function) for manipulating commands, it would be best to place a function header for it with the other function headers in command.h so that users of your command library can call it. Header files are widely used in most C programs.
Header files are included (essentially programmatically copy-pasted) by the #include directive you often see at the top of C source files.

