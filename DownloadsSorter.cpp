#include <filesystem>
#include <iostream>
#include <string>


namespace fs = std::filesystem;

int main() {

	std::string path;
	std::cout << "Enter path to downloads folder: ";
	std::getline(std::cin, path);

	fs::path downloads = path;

	if (!fs::exists(downloads)) {
		std::cout << "This folder doesn't exist.\n";
	}
	
	if (!fs::is_directory(downloads)) {
		std::cout << "This isn't a directory.\n";
	}

	for (const auto& entry : fs::directory_iterator(downloads)) {
		if (entry.is_regular_file()) {
			std::cout << entry.path().extension().string() << "\n";
		}
	}
	return 0;
}