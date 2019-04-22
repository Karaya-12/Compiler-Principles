#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc, char const *argv[])
{
    /*Get Current Working Directory*/
    // Change CLion Run/Debug Configurations - Working Directory to Current Directory
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    cout << "Current Working Directory: " << cwd << "\n";

    /*Generate Project IO Files Directory*/
    string filePath = "/IO_Text/Source_Code.txt";
    string IOPath = cwd;
    IOPath += filePath;
    cout << "Current IO Path: " << IOPath << endl
         << endl;

    /*Read Local Testing Text File*/
    ifstream(IOFile);
    IOFile.open(IOPath);
    if (IOFile.is_open())
    {
        string line;
        cout << "Content In Current File As Shown Below\n";
        while (!IOFile.eof())
        {
            getline(IOFile, line);
            cout << line << endl;
        }
    }
    else
    {
        cout << "Unable to Open The File...\n";
        exit(-1);
    }
    IOFile.close();
    cout << "File Has Been Closed...\n";
    return 0;
}
