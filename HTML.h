#ifndef HTML_H
#define	HTML_H
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class HTML {
    
public:
    
    HTML ( string filepath );
    
    void setFilePath ( string filepath );
    
    void showStats ( int option, string msg );
    
private:
    
    string filePath, stats;
    
    string genHTML ();
    
    void writeFile ( string data );  
    
    string getTime (); 
};

#endif	