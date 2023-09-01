// build.rs

fn main() {
    println!("cargo:rustc-link-lib=static=glmlib");  // Link the static Fortran library
    println!("cargo:rustc-link-search=native=C:\\Program Files (x86)\\Intel\\oneAPI\\compiler\\2023.2.1\\windows\\compiler\\lib\\intel64_win");
}
