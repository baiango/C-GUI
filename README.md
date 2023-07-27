# ODIN-GUI
I am attempting to make an editor for Bevy. I am fleeing to Bevy now.  
Because Godot codebase are like... There's a lot of inlines, pointers, generic, classes abuses, dead codes, and nice function names.  
[Try to read this lol!!!(Godot)](https://github.com/godotengine/godot-cpp/blob/3162be28e594bf5b17889117670fc6f2d75f2f0c/include/godot_cpp/templates/cowdata.hpp#L141)  
[Compared to this!!!(Bevy)](https://github.com/bevyengine/bevy/blob/eb485b1acc619baaae88d5daca0a311b95886281/crates/bevy_render/src/texture/image.rs#L266C1-L271C6)

# Editor architecture
[You can use this first. (bevy_editor_pls)](https://github.com/jakobhellermann/bevy_editor_pls)  
I use C because of the simplicity, which is critical for GUI program. And Rust/C++ is too verbose for the job because of the namespaces.  
So Rust is only used for long computation time things/algorithms.

# Coding style
[Read here](coding_style.md)  

# Memory-safety
I take memory leaks very seriously. So please use [Intel Inspector](https://www.intel.com/content/www/us/en/docs/inspector/get-started-guide/2023-1/overview.html) to debug this software.  

- There is no function that use pointers in this software except things that depends on Vulkan or external libraries or arrays.  
At the cost of memory bandwidth because of copying the variables.  
But this type of problem will usually get optimized by the compiler.  
- `You need DDR5 to run this software.` I said that as a joke. 
- if/else statement is rarely used to make this software very predictable.
- Same input always return the same output because of the pure functions.
- Writing unit test are much easier as you don't have to write it for if/else statement.  

**So this software is very memory-safe.**  
