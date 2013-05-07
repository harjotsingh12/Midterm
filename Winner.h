#ifndef WINNER_H
#define	WINNER_H
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class Winner {
    
public:
    
    Winner ( int totRows, int totCols );    
    
    void setRows ( int totRows );
    
    void setColumns ( int totCols );
    
    int getRows ();
    
    int getColumns ();
    
    void addInTopToBtmLst ( int gridRowNum, int gridColNum );
    
    void addInLeftToRtLst ( int gridRowNum, int gridColNum );    
    
    void addInTopToBtmLst ( int number );
    
    void addInLeftToRtLst ( int number );  
    
    bool didVWin ();
    
    bool didHWin ();
    
    bool isNumInHList ( int number );    
     
    bool isNumInVList ( int number );
    
    bool isInWinnerList ( int gridRowNum, int gridColNum );
    
    int lastPosOfH ();
    
    int lastPosOfV ();
    
    void print (); //For testing
    
    void getRowAndCol ( int number, int &row, int &col );
    
    void reset ();
    
private:
    
    vector<int> topToBtmList, leftToRtList, 
            topToBtmPath, leftToRtPath;
    
    int rows, columns;
    
    bool vWon, hWon;
    
    int getCellNumber ( int gridRowNum, int gridColNum );
    
    bool numberExists ( int number, vector<int> list );
    
    void removeNumber ( int number, vector<int> &list );
    
    bool checkTopToBottom ( int number, bool checkLeft, bool checkRight );    
    
    bool checkLeftToRight ( int number, bool checkUp, bool checkDown ); 
    
    int getRowNum ( int number );
    
};

#endif	