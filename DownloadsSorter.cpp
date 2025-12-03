#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>
#include <iostream>
#include <string>
#include <cctype> // for tolower()
#include <cstdlib> // for std::getenv

namespace fs = std::filesystem;

fs::path get_default_downloads_folder() {
#ifdef _WIN32
	if (const char* home = std::getenv("USERPROFILE")) { // if (home != nullptr)
		// C:\users\username\Downloads
		return fs::path(home) / "Downloads";
	}
#endif
	return {}; //empty
}

// classification ext into different types and categories
std::string classify_extension(const fs::path& p) {

	std::string ext = p.extension().string();

	// lower reg
	for (char& ch : ext) {
		ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
	}

	// types
	if (ext == ".pdf" || ext == ".djvu" || 
		ext == ".doc" || ext == ".docx" || 
		ext == ".pptx" || ext == ".xlsx") {
		return "Document";
	}

	if (ext == ".jpg" || ext == ".jpeg" || ext == ".png") {
		return "Image";
	}

	if (ext == ".mp3" || ext == ".flac" || ext == ".wav") {
		return "Music";
	}

	if (ext == ".mp4" || ext == ".avi" || 
		ext == ".mkv" || ext == ".webm") {
		return "Video";
	}

	if (ext == ".zip" || ext == ".7z" || ext == ".rar") {
		return "Archive";
	}

	if (ext == ".epub" || ext == ".fb2" || ext == ".mobi") {
		return "Book";
	}

	if (ext == ".torrent") return "Torrent";

	if (ext == ".exe") return "Executable";

	return "Other";
}

int main() {

	std::cout << "=== File Sorter ===\n";
	std::cout << "1) Use detected Downloads folder\n";
	std::cout << "2) Enter folder path manually\n";
	std::cout << "0) Exit\n";
	std::cout << "Choose option: ";

	int option = 0;
	if (!(std::cin >> option)) {
		std::cout << "Invalid input.\n";
		return 1;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clear buffer

	fs::path downloads;

	if (option == 0) {
		std::cout << "Bye!\n";
		return 0;
	}
	else if (option == 1) {
		downloads = get_default_downloads_folder();
		if (downloads.empty()) {
			std::cout << "Could not detect Downloads folder.\n";
			return 1;
		}
	}
	else if (option == 2) {
		std::cout << "Enter path to folder: ";
		std::string path;
		std::getline(std::cin, path);
		downloads = path;
	}
	else {
		std::cout << "Unknown option.\n";
		return 1;
	}

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