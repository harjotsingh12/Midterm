#include "HTML.h"

//PUBLIC FUNCTIONS
HTML::HTML ( string filepath )
{
    string cmd;
    filePath = filepath;
    stats = "";
    
    cmd = "rm " + filePath;
    system ( cmd.c_str () );
    
    cmd = "touch " + filePath;
    system ( cmd.c_str () );
}

void HTML::setFilePath ( string filepath )
{
    string cmd;
    filePath = filepath;
    
    cmd = "rm " + filePath;
    system ( cmd.c_str () );
    
    cmd = "touch " + filePath;
    system ( cmd.c_str () );
}

string HTML::genHTML ()
{
    string htmlData = "";
        
    htmlData += "<HEAD>";
    htmlData += "<title>CMPS 335 MIDTERM</title>";
    htmlData += "</HEAD>";
        
    htmlData += "<body bgcolor=\"#FFBA8D\">";
    htmlData += "<b><center><font size=\"7\">Statistics</font></center></b>";            
    htmlData += stats;
    htmlData += "</body>";
    htmlData += "</HTML>";
     
    return htmlData;
}

void HTML::showStats ( int option, string msg )
{
    if ( msg == "" )
    {
        switch ( option ) 
        {
            case 1:
                stats += "<br> </br>";
                stats += "<br>*****Two players started playing against\
                                    each other at "+getTime()+"*****</br>";
                break;
                
            case 2:
                stats += "<br> </br>";
                stats += "<br>*****User started playing against computer\
                                    as Victor at "+getTime()+"*****</br>";
                break;
                
            case 3:
                stats += "<br> </br>";
                stats += "<br>*****User started playing against computer\
                                    as Hank at "+getTime()+"*****</br>";
                break;
        }
        
    }else
    {
        stats += "<br>" + msg + " ("+getTime()+")</br>";
    }
    
    writeFile ( genHTML () );
}

//PRIVATE FUNCTIONS
void HTML::writeFile ( string data )
{
    ofstream myfile;   
    
    myfile.open ( filePath.c_str () );
    
    if ( myfile.is_open () )
    {
        myfile << data;
        myfile.close ();
    }else
    {
        cout << "Couldn't open " << filePath << endl;
    }
}

string HTML::getTime ()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    return asctime ( timeinfo );
}