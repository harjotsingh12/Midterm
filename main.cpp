#include <stdio.h>
#include <stdlib.h>
#include "glfw.h"
#include <time.h>
#include <unistd.h>
#include <pthread.h> 
#include "Winner.h"
#include "HTML.h" 
#include "AI.h"
using namespace std;

//macros
#define rnd () ( double ) rand () / ( double ) RAND_MAX

//prototypes
void init ( void );
int init_glfw ( void );
void init_opengl ( void );
void drawBackground ( void );
void drawGameScreen ( void );
void GLFWCALL mouse_click ( int button, int action );
void checkKeyboard ( bool &go );
void check_mouse ( void );
void get_grid_center ( const int i, const int j, int cent[2] );
GLuint loadBMP ( const char *imagepath );
void DrawImage ( GLuint num, float x1, float x2, float y1, float y2 );
void reset ();
void *startThread ( void *threadid );

int xres = 640;
int yres = 480;
        
Winner winner ( 4, 4 );
AI ai;
HTML html ( "index.html" );

bool playWithCompHank = false;
bool playWithCompVictor = false;

typedef struct t_grid {
	int status;
	int over;
	float color[4];
	struct t_grid *prev;
	struct t_grid *next;
} Grid;

Grid grid[4][4];
int grid_dim = 4;
int board_dim;
int qsize;
//
int lbutton = 0;
int rbutton = 0;
//
GLuint Htexture;
GLuint Vtexture;
GLuint Menu;
GLuint boardOptions;
GLuint VictorWon;
GLuint HankWon;

int b2;
int screen_center[2];
int s0 ;
int s1 ;
int bq, bp;
int quad[2];
int cent[2];

struct Screen {
    int number;
    int nextScreen;
};
Screen *screen= new Screen;

int main(void)
{
    bool go = true;
    screen->number = 1;
    if ( init_glfw () )
        
    {
        exit ( EXIT_FAILURE );
    }
    
    init_opengl ();
    init ();
    srand ( ( unsigned int ) time ( NULL ) );
        
    while ( go )
    {
        check_mouse ();                
	drawBackground ();                
                
        switch ( screen->number )
        {
            case 1:
                DrawImage ( Menu, 120, 1180, 700, 60 );
                break;
                        
            case 2:
                drawGameScreen ();
                glColor3f ( 0.5f, 0.1f, 0.1f );
                DrawImage ( boardOptions, 120, 350, 670, 95 );       
                break;
                        
            case 3:
                DrawImage ( VictorWon, 120, 1180, 700, 60 );
                break;
                       
            case 4:
                DrawImage ( HankWon, 120, 1180, 700, 60 );
                break;                        
            }
                
	    glfwSwapBuffers ();
	    checkKeyboard ( go );
            
	    if ( !glfwGetWindowParam ( GLFW_OPENED ) ) break;
	}       

        delete screen;
	glfwTerminate ();
	exit ( EXIT_SUCCESS );
}

int init_glfw ( void )
{
    int nmodes;
    GLFWvidmode glist[256];
    
    if ( !glfwInit () )
    {
        printf ( "Failed to initialize GLFW\n" );
	return 1;
    }
    
    //get the monitor native full-screen resolution
    nmodes = glfwGetVideoModes ( glist, 250 );
    xres = glist[nmodes-1].Width;
    yres = glist[nmodes-1].Height;
    //create a window
    //if (!glfwOpenWindow(xres, yres, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
    if ( !glfwOpenWindow ( xres,yres,8,8,8,0,32,0,GLFW_FULLSCREEN ) )
    {
        glfwTerminate ();
	return 1;
    }
    
    glfwSetWindowTitle ( "Game Board and Grid" );
    glfwSetWindowPos ( 0, 0 );
    //make sure we see the escape key pressed
    glfwEnable ( GLFW_STICKY_KEYS );
    glfwSetMouseButtonCallback ( mouse_click );
    glfwEnable ( GLFW_MOUSE_CURSOR );
    //enable vertical sync (on cards that support it)
    glfwSwapInterval ( 1 );
    return 0;
}

void init_opengl ( void )
{
    //OpenGL initialization
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth ( 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT );
    glEnable ( GL_COLOR_MATERIAL );
    //
    //choose one of these
    //glShadeModel(GL_FLAT);
    glShadeModel ( GL_SMOOTH );
    glDisable ( GL_LIGHTING );
    glBindTexture ( GL_TEXTURE_2D, 0 );
    //
    glEnable ( GL_TEXTURE_2D );
        
    Htexture = loadBMP ( "H.bmp" );
    Vtexture = loadBMP ( "V.bmp" );
    Menu = loadBMP ( "Menu.bmp" );
    VictorWon = loadBMP ( "VictorWon.bmp" );
    HankWon = loadBMP ( "HankWon.bmp" );
    boardOptions = loadBMP ( "boardOptions.bmp" );
                
    glBindTexture ( GL_TEXTURE_2D, 0 ); 
    printf ( "tex: %i %i\n", Htexture, Vtexture );
}

void init ( void )
{
    board_dim = yres - 200;
    //make board dim divisible by 4
    board_dim >>= 2;
    board_dim <<= 2;
    int b2 = board_dim/2;
    int bq, bp;
    //quad upper-left corner
    //bq is the width of one grid section
    bq = ( board_dim >> 2 );
    qsize = ( bq - 10 ) / 2;
    //
    //notes:
    //This code is not generic.
    //A goal for this project is to make your board-size generic.
    //Allow the user to select the grid dimensions.
    //For instance, 8x8 board, or 20x20 board.
    //Maybe get this parameter at the command-line.
}

void checkKeyboard ( bool &go )
{
    if ( glfwGetKey ( 'Q' ) == GLFW_PRESS || glfwGetKey ( 'q' ) == GLFW_PRESS )
    {
        go = false;
        
    }else if ( glfwGetKey ( '1' ) == GLFW_PRESS && screen->number == 1 )
    {
        screen->number = 2;
        playWithCompHank = false;
        playWithCompVictor = false;
        html.showStats ( 1, "" );
        
    }else if ( glfwGetKey ( '2' ) == GLFW_PRESS && screen->number == 1 )
    {
        screen->number = 2;
        playWithCompHank = false;
        playWithCompVictor = true;
        html.showStats ( 2, "" );
        
    }else if ( glfwGetKey ( '3' ) == GLFW_PRESS && screen->number == 1 )
    {
        screen->number = 2;
        playWithCompHank = true;
        playWithCompVictor = false;
        html.showStats ( 3, "" );
        
    }else if ( ( glfwGetKey ( 'S' ) == GLFW_PRESS 
            || glfwGetKey ( 's' ) == GLFW_PRESS) && screen->number == 2 )
    {
        screen->number = 1;
        reset ();
        html.showStats ( -1, "Players stopped playing" );
        
    }else if ( ( glfwGetKey ( 'M' ) == GLFW_PRESS
            || glfwGetKey ( 'm' ) == GLFW_PRESS ) 
            && ( screen->number == 3 || screen->number == 4 ) )
    {
        screen->number = 1;
    }
}

void check_mouse ( void )
{
    if ( screen->number != 2 ) return;
    
    static int sx = 0, sy = 0;	
    int x, y;
    int i, j;
    int cent[2];
    lbutton = 0;
    rbutton = 0;
    
    if ( glfwGetMouseButton ( GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS )
    {
        //left mouse button is pressed.
	lbutton = 1;
    }
    
    if ( glfwGetMouseButton ( GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS )
    {
        //right mouse button is pressed.
	rbutton = 1;
    }
    
    glfwGetMousePos ( &x, &y );
    //reverse the y position
    y = yres - y;
    if ( x == sx && y == sy ) return;
    
    sx = x;
    sy = y;	
	
    //
    //is the mouse over any grid squares?
    //
    for ( i = 0; i < 4; i++ )
    {
        for ( j = 0; j < 4; j++ )
        {
            grid[i][j].over = 0;
	} 
    }
    
    for ( i = 0; i < 4; i++ )
    {
        for ( j = 0; j < 4; j++ )
        {
            get_grid_center ( i, j, cent );            
            
	    if ( x >= cent[0] - qsize && x <= cent[0] + qsize &&
                    y >= cent[1] - qsize && y <= cent[1] + qsize )
            {
                grid[i][j].over = 1;
		break;
                //You could do a return here.
		//If more code is added below, a return
		//would cause you to exit too early.
	    }
        }
        
        if ( grid[i][j].over ) break;
    }
}

void GLFWCALL mouse_click ( int button, int action )
{
    if ( screen->number != 2) return;
    
    int x, y;
    
    if ( action == GLFW_PRESS )
    {
        int i, j, k = 0;
        
	//center of a grid
	int cent[2];
	glfwGetMousePos( &x, &y );
	
        //reverse the y position
	y = yres - y;
        
	for ( i = 0; i < 4; i++ )
        {
            for ( j = 0; j < 4; j++ )
            {  
                get_grid_center ( i, j, cent );
                
                if ( x >= cent[0] - qsize && x <= cent[0] + qsize &&
                        y >= cent[1] - qsize && y <= cent[1] + qsize)
                {
                    if ( button == GLFW_MOUSE_BUTTON_LEFT )
                    {           
                        if ( playWithCompHank ) return;
                        
                        grid[i][j].status = 1;
                        winner.addInTopToBtmLst ( i, j ); 
                                            
                        if ( winner.didVWin () )
                        {
                            cout << "Victor WON!!" << endl;
                            html.showStats ( -1, "Victor Won!" );
                            screen->nextScreen = 3;
                            
                            pthread_t thread;
                            pthread_create ( &thread, NULL, startThread,\
                                    ( void * ) screen);                            
                                          
                            return;
                        }
                                            
                        if ( playWithCompVictor ) {
                            int num, row, col;                                                
                                            
                            ai.setTopToBtm ( false );
                            num=ai.play ( winner );
                                                                                            
                            winner.getRowAndCol ( num, row, col );
                            grid[row][col].status = 2;
                                            
                            if ( winner.didHWin () )
                            {
                                cout << "Hank Won!" << endl;
                                html.showStats ( -1, "Hank Won!" );
                                
                                screen->nextScreen = 4;
                            
                                pthread_t thread;
                                pthread_create ( &thread, NULL, startThread,\
                                        ( void * ) screen); 
                                
                                return;
                            } 
                        }
                                            
                    }
                    
                    if ( button == GLFW_MOUSE_BUTTON_RIGHT )
                    {
                        if ( playWithCompVictor ) return;
                                            
                        grid[i][j].status = 2;
                        winner.addInLeftToRtLst ( i, j ); 
                                            
                        if ( winner.didHWin () )
                        {
                            cout << "Hank WON!!" << endl;
                            html.showStats ( -1, "Hank Won!" );
                            
                            screen->nextScreen = 4;
                            
                            pthread_t thread;
                            pthread_create ( &thread, NULL, startThread,\
                                    ( void * ) screen); 
                            return;
                        }
                        
                        if ( playWithCompHank )
                        {
                            int num, row, col;                                                
                                            
                            ai.setTopToBtm ( true );
                            num=ai.play ( winner );
                                                                                            
                            winner.getRowAndCol ( num, row, col );
                            grid[row][col].status = 1;
                                            
                            if ( winner.didVWin () )
                            {
                                cout << "Victor Won!" << endl;
                                html.showStats ( -1, "Victor Won!" );
                                
                                screen->nextScreen = 3;
                            
                                pthread_t thread;
                                pthread_create ( &thread, NULL, startThread,\
                                        ( void * ) screen);                               
                                return;
                            } 
                        }                                
                    }
                    
                    k = 1;
                    break;
	        }
	    }
            
            if ( k ) break;
	}
    }
}

void get_grid_center ( const int i, const int j, int cent[2] )
{
    //make board dim divisible by 4
    board_dim >>= 2;
    board_dim <<= 2;
    //bq is the width of one grid section
    bq = ( board_dim >> 2 );
    //-------------------------------------
    quad[0] = s0 - b2;
    quad[1] = s1 - b2;
    cent[0] = quad[0] + bq/2;
    cent[1] = quad[1] + bq/2;
    cent[0] += ( bq * j );
    cent[1] += ( bq * i );
}

void drawBackground ()
{
    b2 = board_dim / 2;
    screen_center[0] = xres / 2;
    screen_center[1] = yres / 2;
    s0 = screen_center[0];
    s1 = screen_center[1];
	        
    //bq is the width of one grid section
    bq = ( board_dim >> 2 );
    //--------------------------------------------------------
    //start the opengl stuff
    //set the viewing area on screen
    glViewport ( 0, 0, xres, yres );
    //clear color buffer
    glClearColor ( 0.1f, 0.2f, 0.3f, 0.0f );
    glClear ( GL_COLOR_BUFFER_BIT );
    //init matrices
    glMatrixMode ( GL_PROJECTION); glLoadIdentity ();
    glMatrixMode( GL_MODELVIEW ); glLoadIdentity ();
    //this sets to 2D mode (no perspective)
    glOrtho ( 0, xres, 0, yres, -1, 1 );
        
    glColor3f ( 0.8f, 0.6f, 0.2f );
}

void drawGameScreen ()
{	
    //draw stuff
    //draw the main game board in middle of screen
    glBegin ( GL_QUADS );
        glVertex2i ( s0 - b2, s1 - b2 );
	glVertex2i ( s0 - b2, s1 + b2 );
	glVertex2i ( s0 + b2, s1 + b2 );
	glVertex2i ( s0 + b2, s1 - b2 );
    glEnd ();
    
    
    //draw grid lines
    //vertical
    glColor3f ( 0.1f, 0.1f, 0.1f );
    bp = s0 - b2;
    glLineWidth ( 2 );
    
    glBegin ( GL_LINES );
        bp += bq;
	glVertex2i ( bp, s1 - b2 );
	glVertex2i ( bp, s1 + b2 );
	bp += bq;
	glVertex2i ( bp, s1 - b2 );
	glVertex2i ( bp, s1 + b2 );
	bp += bq;
	glVertex2i ( bp, s1 - b2 );
	glVertex2i ( bp, s1 + b2 );
    glEnd ();
    
    //horizontal
    glColor3f ( 0.2f, 0.2f, 0.2f );
    bp = s1 - b2;
    
    glBegin ( GL_LINES );
        bp += bq;
	glVertex2i ( s0 - b2, bp );
	glVertex2i ( s0 + b2, bp );
	bp += bq;
	glVertex2i ( s0 - b2, bp );
	glVertex2i ( s0 + b2, bp );
	bp += bq;
	glVertex2i ( s0 - b2, bp );
	glVertex2i ( s0 + b2, bp );
    glEnd ();
    
    glLineWidth ( 1 );
    //
    //draw a new square in center of each grid
    //squares are slightly smaller than grid
    //      
        
    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            get_grid_center ( i, j, cent );
	    glColor3f ( 0.5f, 0.1f, 0.1f );
            
            if ( grid[i][j].over )
            {
                glColor3f ( 1.0f, 1.0f, 0.0f );
	    }
            
            if ( winner.isInWinnerList ( i, j ) )
            {
                glColor3f ( 0.1f, 0.1f, 0.1f );
            }
                        
	    glBindTexture ( GL_TEXTURE_2D, 0 );
            
	    if ( grid[i][j].status == 1 )
            {
                glBindTexture ( GL_TEXTURE_2D, Vtexture );
            }
            
	    if ( grid[i][j].status == 2 )
            {
                glBindTexture ( GL_TEXTURE_2D, Htexture );
            }
            
	    glBegin ( GL_QUADS );
                glTexCoord2f ( 0.0f, 0.0f );
                glVertex2i ( cent[0] - qsize, cent[1] - qsize );
		glTexCoord2f ( 0.0f, 1.0f ); 
                glVertex2i ( cent[0] - qsize, cent[1] + qsize );
		glTexCoord2f ( 1.0f, 1.0f );
                glVertex2i ( cent[0] + qsize, cent[1] + qsize );
		glTexCoord2f ( 1.0f, 0.0f );
                glVertex2i ( cent[0] + qsize, cent[1] - qsize );
            glEnd ();
            
            glBindTexture ( GL_TEXTURE_2D, 0 );
	}
    }
}

GLuint loadBMP ( const char *imagepath )
{
    //When you create your texture files, please specify
    //type: BMP
    //color depth: 24-bit
    unsigned int retval;
    unsigned char header[54];
    //Each BMP file begins by a 54-bytes header
    //Position in the file where the actual data begins
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    // = width*height*3
    //RGB data will go in this
    unsigned char *data; 
    //
    printf ( "loadBMP(%s)...\n", imagepath );
    //Log("opening file **%s**\n",imagepath);
    
    FILE * file = fopen ( imagepath, "r" );
    if ( !file )
    {
        printf ( "Image could not be opened\n" );
	return 0;
    } 
    
    if ( fread ( header, 1, 54, file ) !=54 )
    {
	// If not 54 bytes read : problem
	printf ( "Not a correct BMP file\n" );
	return 0;
    }
    
    if ( header[0] != 'B' || header[1] != 'M' )
    {
	printf ( "Not a correct BMP file\n" );
	return 0;
    }
    
    dataPos   = *(int*) & ( header[0x0A] );
    imageSize = *(int*) & ( header[0x22] );
    width     = *(int*) & ( header[0x12] );
    height    = *(int*) & ( header[0x16] ); 
    
    //Some BMP files are misformatted, guess missing information
    if ( imageSize == 0 ) imageSize = width * height * 3;
    if ( dataPos == 0 ) dataPos = 54;
    
    data = ( unsigned char * ) malloc ( imageSize + 1 );
    //Read the actual data from the file into the buffer
    retval = fread ( data, 1, imageSize, file );
    fclose ( file ); 
    //In glTexImage2D, the GL_RGB indicates that we are talking
    //about a 3-component color, and GL_BGR says how exactly
    //it is represented in RAM. As a matter of fact, BMP does
    //not store Red->Green->Blue but Blue->Green->Red, so we
    //have to tell it to OpenGL.
    #define GL_BGR 0x80E0
    //Create one OpenGL texture
    GLuint textureID;
    glGenTextures ( 1, &textureID );
    glBindTexture ( GL_TEXTURE_2D, textureID );
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, \
            GL_UNSIGNED_BYTE, data );
    
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); 
    free ( data );
    
    return textureID;
}

void DrawImage( GLuint num, float x1, float x2, float y1, float y2 )
{
    
    glBindTexture ( GL_TEXTURE_2D, num );
    
    glBegin ( GL_QUADS );
      glTexCoord2f ( 0.0f, 1.0f );
      glVertex2f ( x1, y1 );
      glTexCoord2f ( 1.0f, 1.0f );
      glVertex2f ( x2, y1 );
      glTexCoord2f ( 1.0f, 0.0f );
      glVertex2f ( x2, y2 );
      glTexCoord2f ( 0.0f, 0.0f );
      glVertex2f ( x1, y2 );
    glEnd ();
}

void reset()
{
    ai.reset ();
    winner.reset ();
    
    playWithCompHank = playWithCompVictor = false;
    
    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            grid[i][j].status = 0;
        }
    }
}

void *startThread ( void *threadid )
{
    Screen *sc = ( Screen * ) threadid;
    sleep ( 1 );
    sc->number = sc->nextScreen;  
    reset ();
}
