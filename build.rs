use std::fs;
use std::path::Path;
use std::process::Command;


fn main() {
	let output_dir = Path::new("GLMLib\\x64\\Release\\");

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

	// Define the command and its arguments
	let obj_output = Command::new("C:\\Program Files (x86)\\Intel\\oneAPI\\compiler\\2023.2.1\\windows\\bin\\ifx")
		.args(&[
			"/O3",
			format!("/object:{}", output_dir.display()).as_str(),
			"/c",
			"/Qlocation,link,\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.37.32822\\bin\\HostX64\\x64\"",
			"/names:lowercase",
			"GLMLib\\glmath.f90",
		])
		.output()
		.expect("Failed to execute command");

	// Print the output of the command (stdout and stderr)
	println!("stdout: {:?}", String::from_utf8_lossy(&obj_output.stdout));
	println!("stderr: {:?}", String::from_utf8_lossy(&obj_output.stderr));

	// Check the exit status of the command
	if !obj_output.status.success() {
		panic!("Command failed with exit status: {:?}", obj_output.status);
	}

	// Define the command and its arguments
	let lib_output = Command::new("C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.37.32822\\bin\\Hostx64\\x64\\lib")
		.args(&[
			format!("/OUT:{}glmlib.lib", output_dir.display()).as_str(),
			format!("{}glmath.obj", output_dir.display()).as_str(),
		])
		.output()
		.expect("Failed to execute command");

	// Print the output of the command (stdout and stderr)
	println!("stdout: {:?}", String::from_utf8_lossy(&lib_output.stdout));
	println!("stderr: {:?}", String::from_utf8_lossy(&lib_output.stderr));

	// Check the exit status of the command
	if !lib_output.status.success() {
		panic!("Command failed with exit status: {:?}", lib_output.status);
	}

	println!("cargo:rustc-link-lib=static={}glmlib", output_dir.display());  // Link the static Fortran library
	println!("cargo:rustc-link-search=native=C:\\Program Files (x86)\\Intel\\oneAPI\\compiler\\2023.2.1\\windows\\compiler\\lib\\intel64_win");
}
