# C-GUI
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
