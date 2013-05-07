#ifndef AI_H
#define	AI_H
#include <vector>
#include "Winner.h"
using namespace std;

class AI {
    
public:
    
    AI ();
    
    void setTopToBtm ( bool val );
    
    int play ( Winner &winner );
    
    void reset ();
    
private:
    
    bool topToBtm;
    
    int lastNum;
    
    int getRowNum ( int number, int rows, int columns );
    
    int genRandomNum ( Winner winner );
    
    int playTopToBtm ( Winner winner );
    
    int playLeftToRight ( Winner winner );
};

#endif	