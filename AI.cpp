#include "AI.h"

//PUBLIC FUNCTIONS
AI::AI ()
{
    topToBtm = true;
    lastNum = 0;
}

void AI::setTopToBtm ( bool val )
{
    topToBtm = val;
}

int AI::play ( Winner &winner )
{
    int number;
    
    if ( topToBtm ) 
    {
        number = playTopToBtm ( winner );
        if ( number != -1 )
        {
            winner.addInTopToBtmLst ( number );
        }
        
        return number;
        
    }else
    {
        number = playLeftToRight ( winner );
        if ( number != -1) 
        {
            winner.addInLeftToRtLst ( number );
        }
        
        return number;
    }

}

void AI::reset ()
{
    lastNum = 0;
}

//PRIVATE
int AI::getRowNum ( int number, int rows, int columns )
{
    int currRow = 1;
    
    for ( int n = 1; n < rows * columns; n += columns )
    {
        if ( number >= n && number <= n + ( columns - 1 ) )
        {
            return currRow;
        }
        currRow++;
    }
    return -1;
}

int AI::genRandomNum ( Winner winner )
{   
    int num;
    
    if ( topToBtm )
    {
        num = ( rand () % winner.getColumns () ) + 1; 
        
        while ( winner.isNumInHList ( num ) )
        {
            num = ( rand () % winner.getColumns () ) + 1;
        }
        
    }else
    {
        vector<int> vec;        
        int position = rand () % winner.getRows ();
        
        for ( int n = 1; n <= ( ( winner.getRows () - 1 ) *
                winner.getColumns () ) + 1; n += winner.getColumns () )
        {
            vec.push_back ( n );
        }
        
        num = vec.at ( position );
        
        while (winner.isNumInVList ( num ) )
        {
            position = ( rand () % winner.getRows () ) + 1;
            num = vec.at ( position );
        }
    }
    
    return num;
}

int AI::playTopToBtm ( Winner winner )
{  
    int bottomLeft, bottomRight, bottom;
    int rows = winner.getRows ();
    int columns = winner.getColumns ();
    
    if ( lastNum == 0 )
    {
        lastNum = genRandomNum ( winner );
        return lastNum;
    }    
    
    bottomLeft = ( lastNum + columns ) - 1;
    if ( getRowNum ( lastNum, rows, columns) >= 
            getRowNum ( bottomLeft, rows, columns ) 
            || bottomLeft > rows * columns )
    {
        bottomLeft = -1;
    }
    
    bottomRight = ( lastNum + columns ) + 1;
    if ( getRowNum ( bottomRight, rows, columns ) > 
            getRowNum ( lastNum, rows, columns ) + 1 
            || bottomRight > rows * columns )
    { 
        bottomRight = -1;
    }   
    
    bottom = lastNum + columns;
    if ( bottom > rows * columns )
    {
        bottom = -1;
    }
    
    if ( bottom != -1 && !winner.isNumInHList ( bottom ) )
    {        
        lastNum = bottom;
        return lastNum;
        
    }else if ( bottomRight != -1 && !winner.isNumInHList ( bottomRight ) )
    {
        lastNum = bottomRight;
        return lastNum;
        
    }else if ( bottomLeft != -1 && !winner.isNumInHList ( bottomLeft ) )
    {
        lastNum = bottomLeft;
        return lastNum;
        
    }else
    {
        int vPos = winner.lastPosOfH ();
        int nextNum = vPos + 1;
        
        if ( winner.isNumInHList ( nextNum ) || winner.isNumInVList ( nextNum ) )
        {
            nextNum = vPos - 1;
        }
        
        if ( getRowNum ( nextNum, rows, columns ) != 
                getRowNum ( vPos, rows, columns ) || nextNum > rows * columns
                || nextNum < 1 || winner.isNumInHList ( nextNum ) 
                || winner.isNumInVList ( nextNum ) )
        { 
            for ( int n = 1; n <= rows * columns; n++ )
            {
                if ( !winner.isNumInHList ( n ) && !winner.isNumInVList ( n ) )
                {
                    return n;
                }
            }
            
            return -1;
            
        }else
        {
            return nextNum;
        }
    }
}

int AI::playLeftToRight ( Winner winner )
{  
    int straightUp, straightDown, straight;
    int rows = winner.getRows ();
    int columns = winner.getColumns ();
    
    if ( lastNum == 0 )
    {
        lastNum = genRandomNum ( winner );
        return lastNum;
    }    
    
    straightUp = ( lastNum - columns ) + 1;
    if ( getRowNum ( lastNum, rows, columns ) <= 
            getRowNum ( straightUp, rows, columns ) 
            || straightUp < 1 )
    {
        straightUp = -1;
    }
    
    straightDown = ( lastNum + columns ) + 1;
    if ( getRowNum ( lastNum, rows, columns ) >= 
            getRowNum ( straightDown, rows, columns ) 
            || straightDown > rows * columns )
    {
        straightDown = -1;
    }   
    
    straight = lastNum + 1;
    if ( getRowNum ( lastNum, rows, columns ) != 
            getRowNum ( straight, rows, columns ) 
            || straight > rows * columns )
    {
        straight = -1;
    } 
    
    if ( straight != -1 && !winner.isNumInVList ( straight ) )
    {        
        lastNum = straight;
        return lastNum;
        
    }else if ( straightUp != -1 && !winner.isNumInVList ( straightUp ) )
    {
        lastNum = straightUp;
        return lastNum;
        
    }else if ( straightDown != -1 && !winner.isNumInVList ( straightDown ) )
    {
        lastNum = straightDown;
        return lastNum;
        
    }else
    {
        int vPos = winner.lastPosOfV ();
        int nextNum = vPos + columns;       
        
        if ( nextNum > rows * columns || winner.isNumInVList ( nextNum ) 
                || winner.isNumInHList ( nextNum ) )
        { 
            for ( int n = 1; n <= rows * columns; n++ )
            {
                if ( !winner.isNumInVList ( n ) && !winner.isNumInHList ( n ) )
                {
                    return n;
                }
            }
            
            return -1;
            
        }else
        {
            return nextNum;
        }
    }
}