#include "Winner.h"

//PUBLIC FUNCTIONS
Winner::Winner ( int totRows, int totCols )
{
    rows = totRows;
    columns = totCols;
    vWon = hWon = false;
}

void Winner::setRows ( int totRows )
{
    rows = totRows;
}

void Winner::setColumns ( int totCols )
{
    columns = totCols;
}
    
void Winner::addInTopToBtmLst ( int gridRowNum, int gridColNum )
{
    gridRowNum = rows - gridRowNum;
    gridColNum++;
    
    int cellNumber = getCellNumber ( gridRowNum, gridColNum );
    
    if ( numberExists (cellNumber, topToBtmList) ) return;
    
    if ( numberExists ( cellNumber, leftToRtList ) )
    {
        removeNumber ( cellNumber, leftToRtList );
    }
    
    topToBtmList.push_back ( cellNumber );
}
    
void Winner::addInLeftToRtLst ( int gridRowNum, int gridColNum )
{
    gridRowNum = rows - gridRowNum;
    gridColNum++;
    
    int cellNumber = getCellNumber ( gridRowNum, gridColNum );
    
    if ( numberExists ( cellNumber, leftToRtList ) ) return;
    
    if ( numberExists ( cellNumber, topToBtmList ) )
    {
        removeNumber ( cellNumber, topToBtmList );
    }
    
    leftToRtList.push_back ( cellNumber );
    
}
    
void Winner::addInTopToBtmLst ( int number )
{
    topToBtmList.push_back ( number );
}

void Winner::addInLeftToRtLst ( int number )
{
    leftToRtList.push_back ( number );
}
    
bool Winner::didVWin ()
{
    for ( int n = 1; n <= columns; n++ )
    {
        if ( numberExists ( n, topToBtmList ) )
        {
            if ( checkTopToBottom (n, true, true ) )
            {  
                topToBtmPath.insert ( topToBtmPath.begin(), n );
                vWon = true;
                return true;
            }
            topToBtmPath.clear ();
        }
    }
    
    return false;
}

bool Winner::didHWin ()
{
    for ( int n = 1; n <= ( ( rows - 1 ) * columns ) + 1; n += columns )
    {
        if ( numberExists ( n, leftToRtList ) )
        {
            if ( checkLeftToRight ( n, true, true ) )
            {    
                leftToRtPath.insert ( leftToRtPath.begin(), n );
                hWon = true;
                return true;
            }        
            leftToRtPath.clear ();
        }
    }    
    
    return false;
}

bool Winner::isInWinnerList ( int gridRowNum, int gridColNum )
{
    gridRowNum = rows - gridRowNum;
    gridColNum++;
    
    int cellNumber = getCellNumber ( gridRowNum, gridColNum );
    
    if ( vWon )
    {
        return numberExists ( cellNumber, topToBtmPath );
        
    }else if ( hWon )
    {
        return numberExists ( cellNumber, leftToRtPath );
        
    }else
    {
        return false;
    }
    
}

void Winner::print () //For testing
{ 
    cout << "TOP TO BOTTOM LIST" << endl;
    for ( int n = 0; n < topToBtmList.size (); n++ )
    {
        cout << topToBtmList.at ( n ) << endl;
    }
    
    cout << "LEFT TO RIGHT LIST" << endl;
    for ( int n = 0; n < leftToRtList.size (); n++ )
    {
        cout << leftToRtList.at ( n ) << endl;
    }
    
    cout << "TOP TO BOTTOM PATH" << endl;
    for ( int n = 0; n < topToBtmPath.size (); n++ )
    {
        cout << topToBtmPath.at ( n ) << endl;
    }
    
    cout << "LEFT TO RIGHT PATH" << endl;
    for ( int n = 0; n < leftToRtPath.size (); n++ )
    {
        cout << leftToRtPath.at ( n ) << endl;
    }
}

int Winner::getRows ()
{
    return rows;
}

int Winner::getColumns ()
{
    return columns;
}

bool Winner::isNumInHList ( int number )
{
    return numberExists ( number, leftToRtList );
}

bool Winner::isNumInVList ( int number )
{
    return numberExists ( number, topToBtmList );
}

int Winner::lastPosOfH ()
{
    return leftToRtList.at ( leftToRtList.size() - 1 );
}

int Winner::lastPosOfV ()
{
    return topToBtmList.at ( topToBtmList.size() - 1 );
}
    
void Winner::getRowAndCol ( int number, int &row, int &col )
{ 
    int currRow=1;
    int currCol=1;
    
    row = col = -1;
    
    for ( int n = 1; n < rows * columns; n += columns )
    {
        if ( number >= n && number <= n + ( columns - 1 ) )
        {           
            while ( n != number )
            {
                n++;
                currCol++;
            }
            break;
        }
        currRow++;
    }
    
    
    row = rows - currRow;
    col = currCol - 1;

}

void Winner::reset ()
{
    vWon = hWon = false;
    
    topToBtmList.clear();
    leftToRtList.clear();
    topToBtmPath.clear();
    leftToRtPath.clear();
}

//PRIVATE FUNCTIONS
int Winner::getCellNumber ( int gridRowNum, int gridColNum )
{
    return ( ( gridRowNum - 1 ) * columns ) + gridColNum;
}

bool Winner::numberExists ( int number, vector<int> list )
{
    if ( find ( list.begin (), list.end (), number ) != list.end () )
    {
        return true;
        
    }else
    {
        return false;
    }

}

void Winner::removeNumber ( int number, vector<int> &list )
{
    list.erase ( remove ( list.begin (), list.end (), number ), list.end () );   
}

bool Winner::checkTopToBottom ( int number, bool checkLeft, bool checkRight )
{
    int left, right, bottomLeft, bottomRight, bottom, lastRowNum;  
    
    lastRowNum = ( rows - 1 ) * columns;
    
                
    left = number - 1;
    if ( getRowNum ( number ) != getRowNum ( left ) || left < 1 )
    {
        left = -1;
    }
    
    right = number + 1;
    if ( getRowNum ( number ) != getRowNum ( right ) 
            || right > rows * columns )
    {
        right = -1;
    }
    
    bottomLeft = ( number + columns ) - 1;
    if ( getRowNum ( number ) >= getRowNum ( bottomLeft ) 
            || bottomLeft > rows * columns )
    {
        bottomLeft = -1;
    }
    
    bottomRight = ( number + columns ) + 1;
    if ( getRowNum ( bottomRight ) > getRowNum ( number ) + 1 
            || bottomRight > rows * columns )
    { 
        bottomRight = -1;
    }   
    
    bottom = number + columns;
    if ( bottom > rows * columns )
    {
        bottom = -1;
    }
    
    if ( numberExists ( number, topToBtmList ) )
    {
        if ( number > lastRowNum )
        {          
            return true;
        }
    }
        
    if ( numberExists ( bottomLeft, topToBtmList ) || 
            numberExists ( bottomRight, topToBtmList ) )
    {
        if ( numberExists ( bottomLeft, topToBtmList ) )
        {                
            if ( checkTopToBottom ( bottomLeft, true, true ) )
            {
                topToBtmPath.push_back ( bottomLeft );
                return true;
                
            }else
            {
                removeNumber ( bottomLeft, topToBtmPath );          
            }
        }
            
        if ( numberExists ( bottomRight, topToBtmList ) )
        {                
            if ( checkTopToBottom ( bottomRight, true, true ) )
            {
                topToBtmPath.push_back ( bottomRight );
                return true;
                
            }else
            {
                removeNumber ( bottomRight, topToBtmPath );
                return false;
            }
        }
            
    }else if ( numberExists ( bottom, topToBtmList ) )
    {            
        if ( checkTopToBottom (bottom, true, true ) )
        {
            topToBtmPath.push_back ( bottom );
            return true;
            
        }else
        {
            removeNumber ( bottom, topToBtmPath );
            return false;
        }
    }
        
    if ( numberExists ( left, topToBtmList ) && checkLeft )
    {            
        if ( checkTopToBottom ( left, true, false ) )
        {
             topToBtmPath.push_back ( left );
             return true;
             
        }else
        {
            removeNumber ( left, topToBtmPath );
        }
    }
        
    if ( numberExists ( right, topToBtmList ) && checkRight )
    {            
        if ( checkTopToBottom ( right, false, true ) )
        {
            topToBtmPath.push_back ( right );
            return true;
            
        }else
        {
            removeNumber ( right, topToBtmPath );
        }
    }  
        
    return false;
    
}

bool Winner::checkLeftToRight ( int number, bool checkUp, bool checkDown )
{
    int up, down, straightUp, straightDown, straight;  
    
    up = number - columns;    
    if ( up < 1 )
    {
        up = -1;
    }
    
    down = number + columns;
    if ( down > rows * columns )
    {
        down = -1;
    }
    
    straightUp = ( number - columns ) + 1;
    if ( getRowNum ( number ) <= getRowNum ( straightUp ) || straightUp < 1 )
    {
        straightUp = -1;
    }
    
    straightDown = ( number + columns ) + 1;
    if ( getRowNum ( number ) >= getRowNum ( straightDown ) 
            || straightDown > rows * columns )
    {
        straightDown = -1;
    }   
    
    straight = number + 1;
    if ( getRowNum ( number ) != getRowNum ( straight ) 
            || straight > rows * columns )
    {
        straight = -1;
    } 
    
    for ( int n = columns; n <= rows * columns; )
    {
        if ( number == n )
        {
            return true;
        }
        n += columns;
    }
        
    if ( numberExists ( straightUp, leftToRtList ) 
            || numberExists ( straightDown, leftToRtList ) )
    {
        if ( numberExists ( straightUp, leftToRtList ) )
        {                
            if ( checkLeftToRight ( straightUp, true, true ) ) 
            {
                leftToRtPath.push_back ( straightUp );
                return true;
                
            }else
            {
                removeNumber ( straightUp, leftToRtPath );          
            }
        }
            
        if ( numberExists ( straightDown, leftToRtList ) )
        {                
            if ( checkLeftToRight ( straightDown, true, true ) )
            {
                leftToRtPath.push_back ( straightDown );
                return true;
                
            }else
            {
                removeNumber ( straightDown, leftToRtPath );
                return false;
            }
        }
            
    }else if ( numberExists ( straight, leftToRtList ) )
    {            
        if ( checkLeftToRight ( straight, true, true ) )
        {
            leftToRtPath.push_back ( straight );
            return true;
            
        }else
        {
            removeNumber ( straight, leftToRtPath );
            return false;
        }
    }
        
    if ( numberExists ( up, leftToRtList ) && checkUp )
    {            
        if ( checkLeftToRight ( up, true, false ) )
        {
             leftToRtPath.push_back ( up );
             return true;
             
        }else
        {
            removeNumber ( up, leftToRtPath );
        }
    }
        
    if ( numberExists ( down, leftToRtList ) && checkDown )
    {            
        if ( checkLeftToRight ( down, false, true ) )
        {
            leftToRtPath.push_back ( down );
            return true;
            
        }else
        {
            removeNumber ( down, leftToRtPath );
        }
    }  
        
    return false;    
}

int Winner::getRowNum ( int number )
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

