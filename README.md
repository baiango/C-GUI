# Fust-GUI
I am attempting to make a Scratch knockoff. It will look like a Blender-style UI. The target users are rapid UI prototyping.  
This project was here because almost every tool distracts you with project folder setup when you only care how about the UI looks and throw it away if it is not fitting.

## How to compile
`cargo r`  
It uses settings from build.rs and cargo.toml. It also relies on the ifx compiler(Intel Fortran Compiler) other than Rustc for the Fortran codes.  
So if the build failed, it mostly is build.rs causing the issue. Because build.rs is handling compiling the Rust and the Fortran code.

## Coding style
[Read here](coding_style.md)  
The coding style is there because it was designed to deal with some of the source editors with lack of autocomplete.  
You can easily write the code without the autocomplete because the codes are consistent.
