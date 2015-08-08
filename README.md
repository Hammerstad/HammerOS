# HammerOS

HammerOS is an operating system for the x86 architecture. 
I challenged myself to program something way out of my comfort zone, and this is the result. 
I knew little to no C/assembly before starting this project (and I still do).

## Inspiration

 - [Bran's Kernel Development Tutorial](http://www.osdever.net/bkerndev/index.php)
 - [JamesM's kernel development tutorials](http://www.jamesmolloy.co.uk/tutorial_html)
 - [Modern Operating Systems](http://www.amazon.com/Modern-Operating-Systems-3rd-Edition/dp/0136006639)
 
## Usage
 
I use the makefile for most use cases, with the exception of building a cross compiler. You have to do that first.
 
### Cross compiler 
 
For usage of the toolchain.sh which builds the cross compiler, type:
 
    sh tools/toolchain.sh
	
In order to build a cross compiler for x86/ia32 architecture type:

    # Install required programs with apr-get, such as gcc, bison, yac and qemu
    # Default installation dir is /usr/local/cross
	# You can override this location by setting the CROSS_PREFIX env variable
	# Remember to alter your makefile accordingly
    sudo bash tools/toolchain.sh install   
	# Compile your very own cross compiler
    sudo bash tools/toolchain.sh ia32
	
### Compile and run HammerOS

Type `make` to get the usages of the makefile:

    help         - shows this help message
    clean        - deletes bin, obj and work directory
    compile      - compiles all files to object files
    iso          - creates an iso image of the kernel
    link         - links all object files together to a binary
    run          - runs the kernel, using qemu

You can compile, link and run HammerOs by typing `make run`.