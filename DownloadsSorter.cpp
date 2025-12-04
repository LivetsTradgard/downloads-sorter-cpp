#define _CRT_SECURE_NO_WARNINGS

#include <limits>
#include <vector>
#include <filesystem>
#include <iostream>
#include <string>
#include <cctype> // for tolower()
#include <cstdlib> // for std::getenv

namespace fs = std::filesystem;

// dublicates searher
fs::path make_unique_target(const fs::path& target_dir,
	const fs::path& filename) {
	fs::path candidate = target_dir / filename;

	if (!fs::exists(candidate)) {
		return candidate;
	}

	// if file exists, add _1, _2
	std::string base = filename.stem().string();      // without ext
	std::string ext = filename.extension().string(); // ext
	int counter = 1;

	while (true) {
		fs::path new_name = base + "_" + std::to_string(counter) + ext;
		candidate = target_dir / new_name;
		if (!fs::exists(candidate)) {
			return candidate;
		}
		++counter;
	}
}





// get path
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

	int docs = 0;
	int images = 0;
	int music = 0;
	int videos = 0;
	int archives = 0;
	int books = 0;
	int torrents = 0;
	int executables = 0;
	int others = 0;

	std::vector<fs::path> files;


	for (const auto& entry : fs::directory_iterator(downloads)) {
		if (entry.is_regular_file()) {
			fs::path path = entry.path(); 
			files.push_back(path);

			std::string category = classify_extension(path);

			std::cout << path.extension().string()
				<< " -> " << category << "\n";

			if (category == "Document")      ++docs;
			else if (category == "Image")    ++images;
			else if (category == "Music")    ++music;
			else if (category == "Video")    ++videos;
			else if (category == "Archive")  ++archives;
			else if (category == "Book")     ++books;
			else if (category == "Torrent")  ++torrents;
			else if (category == "Executable") ++executables;
			else                             ++others;
		}
	}


	std::cout << "\n=== Summary ===\n";
	std::cout << "Documents:   " << docs << "\n";
	std::cout << "Images:      " << images << "\n";
	std::cout << "Music:       " << music << "\n";
	std::cout << "Video:       " << videos << "\n";
	std::cout << "Archives:    " << archives << "\n";
	std::cout << "Books:       " << books << "\n";
	std::cout << "Torrents:    " << torrents << "\n";
	std::cout << "Executables: " << executables << "\n";
	std::cout << "Other:       " << others << "\n";

	std::cout << "\nSort files into subfolders by category? (y/n): ";
	char answer = 'n';
	if (!(std::cin >> answer)) {
		std::cout << "Input error. Exiting.\n";
		return 1;
	}

	if (answer != 'y' && answer != 'Y') {
		std::cout << "Sorting cancelled.\n";
		return 0;
	}

	// sort itself
	for (const auto& path : files) {
		std::string category = classify_extension(path);
		std::string folder_name = category;
		fs::path target_dir = downloads / folder_name;

		try {
			fs::create_directories(target_dir);
			fs::path filename = path.filename();
			fs::path target = make_unique_target(target_dir, filename);

			fs::rename(path, target);

			std::cout << "Moved: "
				<< path.extension().string()
				<< " -> " << folder_name << "\n";
		}
		catch (const std::exception& ex) {
			std::cout << "Failed to move file with extension "
				<< path.extension().string()
				<< ": " << ex.what() << "\n";
		}

	}

	return 0;
}