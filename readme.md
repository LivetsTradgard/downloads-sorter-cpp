# File Sorter (C++17, Windows)

A simple console utility written in modern C++17 that scans a folder, classifies files by extension, shows statistics, and optionally sorts files into subfolders by category.  
The project was created as a learning exercise to practice `<filesystem>` and basic file operations in C++.

---

## Features

- Automatically detects the Windows Downloads folder.
- Allows manual folder selection.
- Classifies files into categories:
  - Document (.pdf, .djvu, .doc, .docx, .pptx, .xlsx)
  - Image (.jpg, .jpeg, .png)
  - Music (.mp3, .flac, .wav)
  - Video (.mp4, .avi, .mkv, .webm)
  - Archive (.zip, .7z, .rar)
  - Book (.epub, .fb2, .mobi)
  - Torrent (.torrent)
  - Executable (.exe)
  - Other (everything else)
- Prints classification for each file in the form "extension -> category".
- Shows a summary with counters for each category.
- Optional sorting step:
  - Creates subfolders inside the selected directory (Document, Image, Music, etc.).
  - Moves files into their corresponding category folders.
  - Avoids overwriting existing files by generating names like name_1.ext, name_2.ext and so on.

---

## How It Works

- The user selects a folder:
  - either the automatically detected Downloads folder,
  - or any folder path entered manually.
- The program scans only the top-level files in the selected folder (no recursion into subdirectories).
- For each file, the program:
  - extracts the extension,
  - normalizes the extension to lowercase,
  - maps the extension to a category,
  - prints "extension -> category",
  - updates the counters for the detected category.
- After scanning, the program prints a summary with the number of files in each category.
- Then the program asks the user whether to sort files into subfolders.
- If the user agrees, the program:
  - creates category subfolders if they do not exist,
  - chooses a unique target name for each file,
  - moves the file into its category folder.

As a result, the selected folder is reorganized into subfolders grouped by file type.

---

## Build Instructions

### Requirements

- Windows
- Visual Studio 2019 or 2022
- C++17 support
- Standard library support for `<filesystem>`

### Building with Visual Studio

- Create a new Console Application project.
- Open Project → Properties → C/C++ → Language.
- Set the C++ language standard to "ISO C++17".
- Replace the default source file with the main source file from this repository.
- In the Configuration Manager, select the Release configuration.
- Build the project.
- Run the generated executable from the Release output folder (for example, x64/Release).

---

## Usage

- Run the executable.
- In the menu, choose one of the options:
  - "Use detected Downloads folder"
  - "Enter folder path manually"
  - "Exit"
- The program scans all regular files in the chosen folder and prints lines like:
  - ".pdf -> Document"
  - ".png -> Image"
  - ".zip -> Archive"
- After the scan, the program prints a summary with the number of files in each category.
- Then the program asks:
  - "Sort files into subfolders by category? (y/n)"
- If the user answers "y" or "Y", files are moved into subfolders:
  - Document, Image, Music, Video, Archive, Book, Torrent, Executable, Other.

---

## Notes

- Only the top level of the selected folder is processed; subdirectories are not scanned.
- The program does not print full filenames to avoid encoding issues with non-ASCII names in the Windows console; only extensions and categories are shown.
- When a file with the same name already exists in the target folder, the program generates a new unique name by appending an index.

---

## Possible Future Improvements

- Recursive folder scanning (including subdirectories).
- User-configurable categories and extension lists (for example, via a config file).
- "Dry run" mode that only prints planned actions without moving files.
- Logging all operations to a log file.
- Better Unicode handling and richer console interface.
- Simple GUI version using WinAPI or Qt.

---
