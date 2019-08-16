# Snake

This version of snake makes use of the Windows API.

![Snake](/.screenshots/Snake1.png?raw=true "Snake")

### Controls
* W to move upward.
* A to move left.
* S to move down.
* D to move right.

Pressing two of the above keys that are in non-opposite directions
will make the snake travel in a diagonal line.


## Build instructions

**Note**: you will need the Windows SDK installed.

### Command Line Visual C++ 

1. Download and extract the source files.

2. Open the **Developer Command Prompt** for Visual Studio or **x86 Native Tools Command Prompt** or **x64 Native Tools Command Prompt**. 

3. Navigate to the source file directory on the developer command prompt using the `cd ` command.

4. Enter `cl /EHsc /O2  main.cpp /Fe"Snake"` into the developer command prompt.


