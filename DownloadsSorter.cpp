#include <filesystem>
#include <iostream>
#include <string>


namespace fs = std::filesystem;

// classification ext into different types and categories
std::string classify_extension(const fs::path& p) {

	std::string ext = p.extension().string();

	// lower reg
	for (char& ch : ext) {
		ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
	}

	// types
	if (ext == ".pdf" || ext == ".djvu" || ext == ".doc" || ext == ".docx" || ext == ".pptx") {
		return "Documents";
	}

	// to-do later

	return "Other";
}

int main() {

	std::string path;
	std::cout << "Enter path to downloads folder: ";
	std::getline(std::cin, path);

	fs::path downloads = path;

	if (!fs::exists(downloads)) {
		std::cout << "This folder doesn't exist.\n";
		return 1;
	}
	
	if (!fs::is_directory(downloads)) {
		std::cout << "This isn't a directory.\n";
		return 1;
	}

	for (const auto& entry : fs::directory_iterator(downloads)) {
		if (entry.is_regular_file()) {
			std::string category = classify_extension(entry.path());

			std::cout << entry.path().extension().string()
				<< " -> " << category << "\n";

		}
	}

	return 0;
}