// dato che la logica è semplice, cerco di massimizzare le prestazioni
// array > arraylist (vector) per cache hit e uso dello stack invece che heap

#include "main.h"

void printNotes(std::vector<note> *notes) {
  std::cout << "Notes:\n";
  int i = 0;
  for (note n : *notes) {
    i++;
    std::cout << "Note" << i << '\n';
    std::cout << n.cdate << n.p << '\n' << n.text << "\n\n";
  }
}

void createNote(std::vector<note> *notes, std::string text, priority p) {
  note n(text, p);
  const time_t t = time(NULL);
  strncpy(n.cdate, ctime(&t), 26);
  notes->push_back(n);
}



// SAVE / LOAD NOTES -> E' importante conoscere la dimensione del vettore prima di caricare i dati nel vettore notes
void saveNotes(std::vector<note> *notes) {
  std::ofstream outstream("saved/notes.bin", std::ios::binary);
  uint64_t size = (*notes).size(); //lunghezza del vettore
  outstream.write((const char *)(&size),sizeof(size));
  outstream.write((const char*)notes->data(),size*sizeof(note));
  outstream.close();
}

std::vector<note> loadNotes() 
{
  std::ifstream instream("saved/notes.bin", std::ios::binary);
  uint64_t size;
  instream.read((char*)&size,sizeof(size));
  std::vector<note> newnotes;
  
  instream.read(reinterpret_cast<char *>(newnotes.data()), size*sizeof(note));

  instream.close();
  return newnotes;
}

void anykeyAnswer(const char *request) {
  char response;
  std::cout << request;
  std::cin >> response;
}

bool userAnswer(const char *request) {
  char response;

  std::cout << request;
  std::cin >> response;
  switch (response) {
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

void createFile(const char *path, const char *content,std::ios_base::openmode traits) {
  std::fstream file(path, traits);
  file.write(content, strlen(content));
  file.close();
}

int main() 
{
  // STATE
  bool exit = false;
  bool firstRun = true;
  bool savedExists = false;

  // VECTOR OF NOTES LOADED FROM FILE
  std::vector<note> notes;

  notes=loadNotes();

  std::cout << "\033[2J\033[1;1H";
  std::cout << "Welcome to the to-do list CLI application!\n";

  // CHECK FIRST RUN WITH DUMMY FILE - use C functions for relative paths

 if (std::filesystem::exists(std::filesystem::current_path() / "dummy.txt") == true)
    firstRun = false;

  if (firstRun == true) {
    // CREATE DUMMY FILE TO CHECK FIRST TIME RUN
    createFile("dummy.txt", "I'm a dummy file :)", std::ios::out);
    // CREATE FOLDER WITH SAVED NOTES
    std::filesystem::create_directory(std::filesystem::current_path() / "saved");
    createFile("saved/notes.bin", "Notes are empty! :c", std::ios::out);

    // SECURITY MEASURE TO BE SURE THAT THE FILES EXIST, BOTH FOR FIRST RUN
    if (std::filesystem::exists(std::filesystem::current_path() / "dummy.txt") == false) {
      std::cerr << "Error: dummy file not generated.\n";
      return -1;
    }
    if (std::filesystem::exists(std::filesystem::current_path() / "saved" / "notes.bin") == false) {
      std::cerr << "Error: notes savefile not generated.\n";
      return -1;
    }

    if (userAnswer("New user, print the tutorial? y/Y or n/N\n") == true) {
      std::cout << "\033[2J\033[1;1H";
      std::cout << "The application lets you create, manage, delete notes. "
                   "Each note contains the text, the priority "
                   "(low,medium,high,urgent) and the creation date.\n"
                   "At startup, the notes will be shown as well as the "
                   "operations possible. 1) Create, 2) Manage, 3) Delete. 4) "
                   "Save changes. 5) Quit\n"
                   "1) Create: Text input and set priority (default priority "
                   "is low)\n"
                   "2) Manage: Two operations possible: \n  a) Modify text\n  "
                   "b) Change priority\n"
                   "3) Delete: Deletes note\n"
                   "4) Save changes: Saves the changes of the to-do list, "
                   "confirmation will be asked if notes have been changed.\n";
      anykeyAnswer("Enter any key to close the tutorial...\n");
      // the tutorial WILL be closed.
      std::cout << "\033[2J\033[1;1H";
    }
  }

  while (exit == false) 
  {

    std::cout << "\033[2J\033[1;1H";
    std::cout << "---MAIN MENU---\n";

    printNotes(&notes);

    std::cout << "\n------------------------------------------------------------------------\n";
    int option = 0;
    std::cout << "Choose an operation:\n"
                 "1) Create\n"
                 "2) Manage\n"
                 "3) Delete\n"
                 "4) Save changes\n"
                 "5) Quit\n"
                 "Enter the operation's number...\n";
    std::cin >> option;
    switch (option) 
    {
        case 1: 
        {
            std::cout << "\n----\n";
            std::cout << "Text:\n";
            std::string content;

            //getline e std::cin ignorano i whitespace. Cin legge input fino al token 'whitespace' e lo lascia nello stream.
            std::cin.ignore();
            std::getline(std::cin,content);

            std::cout << "\nPriority? 0-Low, 1-Medium, 2-High, 3-Urgent? (default is low)\n"; // values equal the enumerators
            int i_priority;
            std::cin >> i_priority;
            switch (i_priority) 
            {
            case 0:
                createNote(&notes, content, priority::low);
                break;
            case 1:
                createNote(&notes, content, priority::medium);
                break;
            case 2:
                createNote(&notes, content, priority::high);
                break;
            case 3:
                createNote(&notes, content, priority::urgent);
                break;
            default:
                createNote(&notes, content, priority::low);
                break;
            }
            break;
        }
            case 2:
            break;
            case 3:
            break;
            case 4:
                saveNotes(&notes);
            break;
            case 5:
                notes=loadNotes();
                std::exit(1);
            break;
            default:
            break;
        }

    
  }
}
