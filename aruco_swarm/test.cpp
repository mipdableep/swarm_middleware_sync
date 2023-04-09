#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main()
{
    string fifo_path = "../PIPE";
    ofstream fifo(fifo_path.c_str());

    if (!fifo)
    {
        cerr << "Failed to open named pipe" << endl;
        exit(EXIT_FAILURE);
    }
    string data = "ls -l";
    fifo << data << endl;
    fifo.close();

}