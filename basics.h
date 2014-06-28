#include <iostream>
#include <SDL/SDL.h>
#include <math.h>
#include "cube.h"

using namespace std;

/*Global Declarations*/
SDL_Surface *screen;
SDL_Event event;
extern dpoint dpoints[800][500];

void erase(bool);
void starter();

void Init()
{
    SDL_Init( SDL_INIT_EVERYTHING );
    screen = SDL_SetVideoMode( 800, 500, 32, SDL_SWSURFACE );
    starter();
    erase(true);
}

void starter()
{
    SDL_Surface* hello = NULL;
    hello = SDL_LoadBMP( "starter.bmp" );
    SDL_BlitSurface( hello, NULL, screen, NULL );
    SDL_Flip( screen );
    SDL_Delay( 2000 );
    SDL_FreeSurface( hello );
}


void setpixel(vertex point, Uint32 color=0x00ff0000)
{
    Uint8 r,g,b;
    r=(color & 0x00ff0000)>>16;
    g=(color & 0x0000ff00)>>8;
    b=(color & 0x000000ff);

    Uint32 pixel;
    pixel=SDL_MapRGB(screen->format, r,g,b);

    char* pPosition = ( char* ) screen->pixels;
	pPosition += ( screen->pitch * (int)point.y );
	pPosition += ( screen->format->BytesPerPixel * (int)point.x );
	memcpy ( pPosition , &pixel, screen->format->BytesPerPixel );
}

void setpixel(float x,float y,float r,float g,float b)
{
    Uint32 pixel;
    pixel=SDL_MapRGB(screen->format, r,g,b);

    char* pPosition = ( char* ) screen->pixels;
	pPosition += ( screen->pitch * (int)y );
	pPosition += ( screen->format->BytesPerPixel * (int)x );
	memcpy ( pPosition , &pixel, screen->format->BytesPerPixel );
}

/*
X,Y
setpixel(X,Y,255,0,0)
*/

int getDepth(surface s,int x,int y)
{
    float A=s.A;float B=s.B;float C=s.C;float D=s.D;
    return (-(A*x)-(B*y)-D)/C ;
}


void draw()
{
    for (int x=0;x<800;x++)
    {
        for(int y=0;y<500;y++)
        {
            float i=dpoints[x][y].Intensity;
            float r=i*dpoints[x][y].r;
            float g=i*dpoints[x][y].g;
            float b=i*dpoints[x][y].b;
            setpixel(x,y,r,g,b);
        }
    }
}


void erase(bool check)
{
    SDL_FillRect( screen,&screen->clip_rect, SDL_MapRGB( screen->format, 255,255,255 ) ); //50 50 70
    if(check){

    for(int x=0;x<800;x++)
    for(int y=0;y<500;y++)
    {
            dpoints[x][y].depth=0;
            dpoints[x][y].set=false;
            dpoints[x][y].Intensity=1;
            dpoints[x][y].r=255;
            dpoints[x][y].g=255;
            dpoints[x][y].b=255;
    }

    }

}
