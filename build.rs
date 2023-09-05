use std::fs;
use std::path::Path;
use std::process::Command;


// You might want to compile this twice
// because Rust doesn't wait for the library to compile
fn build(path: &str, arguments: &[&str]) {
	// Define the command and its arguments
	let cmd = Command::new(path)
		.args(arguments)
		.output()
		.expect("Failed to execute command");

	// Print the output of the command (stdout and stderr)
	println!("stdout: {:?}", String::from_utf8_lossy(&cmd.stdout));
	println!("stderr: {:?}", String::from_utf8_lossy(&cmd.stderr));

	// Check the exit status of the command
	if !cmd.status.success() {
		panic!("Command failed with exit status: {:?}", cmd.status);
	}
}


fn main() {
	let output_dir;
	let optimizion_str;
	#[cfg(debug_assertions)] {
	optimizion_str = "/Od";
	output_dir = Path::new("GLMLib\\x64\\Debug\\");
	}
	#[cfg(not(debug_assertions))] {
	optimizion_str = "/O3";
	output_dir = Path::new("GLMLib\\x64\\Release\\");
	}

	// Create the output directory if it doesn't exist
	if !output_dir.exists() {
		fs::create_dir_all(output_dir)
			.expect("Failed to create output directory");
	}

	// Check if the output directory exists
	if output_dir.exists() {
		// Remove the object files (*.obj) in the output directory
		for entry in fs::read_dir(&output_dir).expect("Failed to read output directory") {
			if let Ok(entry) = entry {
				let path = entry.path();
				if let Some(extension) = path.extension() {
					if extension == "obj" {
						fs::remove_file(&path).expect("Failed to remove object file");
					}
				}
			}
		}

		// Remove the library file (glmlib.lib) if it exists
		let lib_file = output_dir.join("glmlib.lib");
		if lib_file.exists() {
				fs::remove_file(&lib_file).expect("Failed to remove library file");
		}
	}

	let ifx_path = "C:\\Program Files (x86)\\Intel\\oneAPI\\compiler\\2023.2.1\\windows\\bin\\ifx";
	let output_obj_str = format!("/object:{}", output_dir.display());
	let ifx_arguments = [
		&optimizion_str,
		"/arch:core-avx2",
		&output_obj_str.as_str(),
		"/c",
		"/Qlocation,link,\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.37.32822\\bin\\HostX64\\x64\"",
		"/names:lowercase",
		"GLMLib\\glmath.f90",
		"GLMLib\\gldebug.f90",
	];

	build(ifx_path, &ifx_arguments);

	let lib_path = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.37.32822\\bin\\Hostx64\\x64\\lib";
	let output_lib_str = format!("/OUT:{}glmlib.lib", output_dir.display());
	let input_obj_str = format!("{}glmath.obj", output_dir.display());
	let lib_arguments: [&str; 2] = [
		&output_lib_str,
		&input_obj_str,
	];

	build(lib_path, &lib_arguments);

	let output_asm_path: &str = &format!("/Fa{}", output_dir.display());
	let ifx_asm_arguments = [
		"/arch:core-avx2",
		&optimizion_str,
		&output_asm_path,
		"/S",
		"/names:lowercase",
		"GLMLib\\glmath.f90",
		"GLMLib\\gldebug.f90",
	];

	build(ifx_path, &ifx_asm_arguments);

	println!("cargo:rustc-cfg=avx2");
	println!("cargo:rustc-cfg=target_feature=\"avx2\"");
	println!("cargo:rustc-link-lib=static={}glmlib", output_dir.display());  // Link the static Fortran library
	println!("cargo:rustc-link-search=native=C:\\Program Files (x86)\\Intel\\oneAPI\\compiler\\2023.2.1\\windows\\compiler\\lib\\intel64_win");
}
