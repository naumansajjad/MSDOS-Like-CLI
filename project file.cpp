#include <iostream>
#include <string>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

void displayWelcomeMessage()
{
    cout << "Welcome to MS-DOS!\n";
    cout << "Type 'help' to see available commands.\n";
}

string getCurrentDirectory()
{
    return fs::current_path().string();
}

bool createDirectory(const string &directoryName)
{
    try
    {
        fs::path currentPath = fs::current_path();
        fs::path newDirectoryPath = currentPath / directoryName;

        if (fs::create_directory(newDirectoryPath))
        {
            cout << "Directory '" << directoryName << "' created successfully.\n";
            return true;
        }
        else
        {
            cerr << "Error: Directory '" << directoryName << "' creation failed.\n";
            return false;
        }
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

bool changeDirectory(const string &newDirectory)
{
    try
    {
        fs::path currentPath = fs::current_path();
        fs::path newPath = currentPath / newDirectory;

        if (fs::exists(newPath) && fs::is_directory(newPath))
        {
            fs::current_path(newPath);
            cout << "Changed directory to " << newPath << "\n";
            return true;
        }
        else
        {
            cerr << "Error: Directory '" << newDirectory << "' does not exist or is not a directory.\n";
            return false;
        }
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Error: " << e.what() << "\n";
        return false;
    }
}

void copyFile(const string &source, const string &destination)
{
    try
    {
        fs::path currentPath = fs::current_path();
        fs::path sourcePath = currentPath / source;
        fs::copy(sourcePath, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        cout << "File copied from " << source << " to " << destination << "\n";
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Error: " << e.what() << "\n";
    }
}

void RenameFile(const string &currentFile, const string &newFile)
{
    try
    {
        fs::path currentPath = fs::current_path();
        fs::path currentFilePath = currentPath / currentFile;
        fs::path newFilePath = currentPath / newFile;
        fs::rename(currentFilePath, newFilePath);
        cout << "File renamed from " << currentFile << " to " << newFile << "\n";
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Error: " << e.what() << "\n";
    }
}

void deleteFile(const string &fileName)
{
    try
    {
        fs::remove(fileName);
        cout << "File '" << fileName << "' deleted successfully.\n";
    }
    catch (const fs::filesystem_error &e)
    {
        cerr << "Error: " << e.what() << "\n";
    }
}

void listDirectory(const string &directory)
{
    cout << "Listing contents of directory: " << directory << "\n";
    for (const auto &entry : fs::directory_iterator(directory))
    {
        cout << entry.path().filename() << "\n";
    }
}

string moveUpDirectory(const string &currentDirectory)
{
    fs::path path(currentDirectory);
    path = path.parent_path();
    fs::current_path(path);
    cout << "Moved up to directory: " << path << "\n";
    return path.string();
}

void MSDOS_Version()
{
    cout << "\nSN_TECHNOLOGIES      [ Version 1.1.2 ]" << endl;
}

void findDirectory(const string &fileName)
{
    fs::path currentPath = fs::current_path();
    fs::path filePath = currentPath / fileName;
    if (fs::exists(filePath) && fs::is_directory(filePath))
    {
        cout << "The directory '" << fileName << "' is present in this directory" << endl;
    }
    else if (fs::exists(filePath) && !fs::is_directory(filePath))
    {
        cout << "The file '" << fileName << "' is present in this directory" << endl;
    }
    else
    {
        cout << "The file or directory '" << fileName << "' is not present in this directory" << endl;
    }
}

int main()
{
    string currentDirectory = getCurrentDirectory();
    string userInput;

    displayWelcomeMessage();

    while (true)
    {
        cout << currentDirectory << "> ";
        getline(cin, userInput);

        istringstream iss(userInput);
        string command;
        iss >> command;

        if (command == "exit")
        {
            break;
        }
        else if (command == "help")
        {
            cout << "Available Commands:\n";
            cout << "ls - List Directory\n";
            cout << "ver - Version Info\n";
            cout << "cd.. - Move to Parent Directory\n";
            cout << "mkdir [directory_name] - Create directory\n";
            cout << "cd [directory_name] - Change directory\n";
            cout << "ren [file_name] [new_file_name] - Rename File\n";
            cout << "copy [file_name] [destination] - Copy File\n";
            cout << "find [file_Name] - Find File In current Directory\n";
            cout << "del [file_name] - Delete file\n";
            cout << "exit - Exit CLI\n";
        }
        else if (command == "mkdir")
        {
            string dirName;
            iss >> dirName;
            createDirectory(dirName);
        }
        else if (command == "cd")
        {
            string newDir;
            iss >> newDir;
            if (changeDirectory(newDir))
            {
                currentDirectory = fs::current_path().string();
            }
        }
        else if (command == "copy")
        {
            string source, destination;
            iss >> source >> destination;
            copyFile(source, destination);
        }
        else if (command == "del")
        {
            string fileName;
            iss >> fileName;
            deleteFile(fileName);
        }
        else if (command == "ren")
        {
            string file_name, new_file_name;
            iss >> file_name >> new_file_name;
            RenameFile(file_name, new_file_name);
        }
        else if (command == "ls")
        {
            listDirectory(currentDirectory);
        }
        else if (command == "cd..")
        {
            currentDirectory = moveUpDirectory(currentDirectory);
        }
        else if (command == "find")
        {
            string fileName;
            iss >> fileName;
            findDirectory(fileName);
        }
        else if (command == "ver")
        {
            MSDOS_Version();
        }
        else
        {
            cout << "Command not recognized. Type 'help' for available commands.\n";
        }
    }

    return 0;
}
