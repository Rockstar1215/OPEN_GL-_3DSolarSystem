# 3D Solar System Simulation

This project was part of my sixth-semester coursework in Computer Graphics.

## 1. Setup Instructions

### 1.1 Installing Libraries on Ubuntu

Libraries can be installed either via the Ubuntu software center or using the command line. We recommend using the command line for a smoother process. To make things easier, we've provided a script named `install-libraries.sh` that automates the entire installation process. Here's how to proceed:

1. Open your terminal and navigate to the directory where the `install-libraries.sh` file is located.
   
   ```
   bash install-libraries.sh
   ```

2. Execute the script with the command above.
3. Enter your password when prompted, and let the script handle the installation of all necessary libraries. If you encounter an issue with the library `libglew1.6-dev` not being found, you may need to install an older version:

   ```
   sudo apt-get install libglew1.5-dev
   ```

4. For other Linux distributions, you can follow a similar process to install the required OpenGL libraries.

### 1.2 Compilation and Execution

To compile the project, you will generally use `g++`, but to simplify the compilation and linking process, we use a `Makefile`. This file contains details about the source files and the libraries that the project depends on.

To compile and link your program, simply run the `make` command in the project directory:

```
make
```

If there are no errors, this will produce an executable file. Running the executable will display three shapes on your screen. If you encounter any syntax errors, you'll need to fix them and rerun the `make` command.

