//dato che la logica è semplice, cerco di massimizzare le prestazioni

//array > arraylist (vector) per cache hit e uso dello stack invece che heap
//

#include "main.h"

void anykeyAnswer(const char* request)
{
    char response;
    std::cout << request;
    std::cin >> response;
}

bool userAnswer(const char* request)
{
    char response;

    std::cout << request;
    std::cin >> response;
    switch (response)
    {
    case 'y':
    case 'Y':
        return true;
    case 'n':
    case 'N':
        return false;
    default:
        return false;
    }
    return false;
}

void createFile(const char* path, const char* content,std::ios_base::openmode traits)
{
    std::fstream file(path,traits);
    file.write(content,strlen(content));
    file.close();
}

int main()
{
    // STATE
    bool exit = false;
    bool firstRun = true;
    bool savedExists=false;

    while (exit == false)
    {
        // main loop
        // windows solution
        // system("cls");

        // cross-platform solution
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Welcome to the to-do list CLI application!\n";

        //CHECK FIRST RUN WITH DUMMY FILE - use C functions for relative paths
        //https://stackoverflow.com/questions/73656671/stdfilesystempath-obtain-relative-path-given-a-base-path-without-temporary-s
        if(std::filesystem::exists(std::filesystem::current_path() / "dummy.txt")==true)
            firstRun=false;

        if (firstRun == true)
        {
            //CREATE DUMMY FILE TO CHECK FIRST TIME RUN
            createFile("dummy.txt","I'm a dummy file :)",std::ios::out);
            //CREATE FOLDER WITH SAVED NOTES
            std::filesystem::create_directory(std::filesystem::current_path() / "saved");
            createFile("saved/notes.txt","Notes are empty",std::ios::out);
            
            //SECURITY MEASURE TO BE SURE THAT THE FILES EXIST, BOTH FOR FIRST RUN 
            if(std::filesystem::exists(std::filesystem::current_path() / "dummy.txt")==false)
            {
                std::cerr << "Error: dummy file not generated.\n";
                return -1;
            }
            if(std::filesystem::exists(std::filesystem::current_path() / "saved" / "notes.txt")==false)
            {
                std::cerr << "Error: notes savefile not generated.\n";
                return -1;
            }

            if(userAnswer("New user, print the tutorial? y/Y or n/N\n")==true)
            {
                std::cout << "\033[2J\033[1;1H";
                std::cout << "The application lets you create, manage, delete notes. Each note contains the text, the priority (low,medium,high,urgent) and the creation date.\n"
                             "At startup, the notes will be shown as well as the operations possible. 1) Create, 2) Manage, 3) Delete. 4) Save changes\n"
                             "1) Create: Text input and set priority (default priority is low)\n"
                             "2) Manage: Two operations possible: \n  a) Modify text\n  b) Change priority\n"
                             "3) Delete: Deletes note\n"
                             "4) Save changes: Saves the changes of the to-do list, confirmation will be asked if notes have been changed.\n";
                anykeyAnswer("Enter any key to close the tutorial\n");
                //the tutorial WILL be closed.
                std::cout << "\033[2J\033[1;1H";
                
            }

        }
        //PRINT NOTES
        std::cout << "---MAIN MENU---\n";
        
        std::fstream fsaved("saved/notes.txt",std::ios::ate);
        int pointerPos = fsaved.tellg();
        std::cout << pointerPos;
        std::string fileNotes(pointerPos,'\0');
        fsaved.seekg(0);
        fsaved.read(&fileNotes[0],pointerPos);
        fsaved.close();
        std::cout << "Notes:\n" << fileNotes;
        






        exit = true;
    }
}
