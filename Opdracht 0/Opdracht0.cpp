#include <iostream>
#include <string>
using namespace std;

string translate(std::string line, string flag){
    string result="";
    int argument = stoi(flag);
    if(argument>255){argument=argument%256;}
    for(unsigned int i=0; i<line.size(); i++){
        char character=line[i];
            result+=character+argument;
        }
        return result;}

    int main(int argc, char *argv[])
    { std::string line;

        if(argc != 2)
        { std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
            return -1; }

        while(std::getline(std::cin, line))
        { std::cout << translate(line, argv[1]) << std::endl; }

        return 0; }