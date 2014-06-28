#include "basics.h"

using namespace std;
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

int main(int argc, char* argv[])
{
    Init();
    char shadding='x';
    vector<string> text_file;
    int quit = 0;
    bool animate=false;
    float angle=0,anglex=0,anglez=0;
    int testing=0;
    float scale=1.0;

    /*Draw Base*/
    cube base;
    base.init(400,250,0,500,500,30,220,220,220); //70 70 70

    int color[5][3]={{150,100,100},{100,150,100},{100,100,150},{150,150,100},{100,100,200}};

    erase(true);
    base.draw();

    int n=0;
    ifstream ifs( "c://test.txt" );
    string temp;

    while( getline( ifs, temp ) )
        text_file.push_back( temp );

    n=text_file.size();
    cube *s[n];

    int arr[5];

    for(int i=0;i<text_file.size();i++)
    {
        for(int i=0;i<5;i++)
        arr[i]=0;
        int in=0;

        for(int j=0;j<text_file.at(i).length();j++)
        {
            int value=text_file.at(i)[j]-48;

            if(value==-16)
            {
            ++in;
            continue;
            }
            arr[in]=(arr[in]*10)+value;
        }

        s[i]=new cube();
        s[i]->init(150+arr[0],235-12,arr[1]-250,arr[2],arr[3],25,color[arr[4]][0],color[arr[4]][1],color[arr[4]][2]);
        s[i]->draw();
    }

    draw();


    /*
    if height=h then centery=240-35-(h/2)
    */


    bool neg=false;
    //base.draw();

    while( !quit )
    {
        while( SDL_PollEvent( &event ) )
        {

            if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_u )
                {
                    quit=1;
                }

                if( event.key.keysym.sym == SDLK_t )
                {
                    animate=!animate;
                }

                 if( event.key.keysym.sym == SDLK_y )
                {
                    erase(true);

                    if(!neg)
                    {
                            base.translate('y','+',10);
                            for(int i=0;i<n;i++)
                            {
                                s[i]->translate('y','+',10);
                            }
                    }
                    else
                    {
                            base.translate('y','-',10);
                            for(int i=0;i<n;i++)
                            {
                                s[i]->translate('y','-',10);
                            }
                    }

//                    base.draw();
//                    for(int i=0;i<n;i++)
//                    {
//                        s[i]->draw();
//                    }
                    draw();
                }

                if( event.key.keysym.sym == SDLK_SPACE )
                {
                    neg=!neg;
                }


                if( event.key.keysym.sym == SDLK_s )
                {
                    if(!neg)
                    scale-=0.125;
                    else
                    scale+=0.125;
                    erase(true);
                    base.scale(scale);
                    for(int i=0;i<n;i++)
                    {
                        s[i]->scale(scale);
                    }
                    draw();
                }

                if( event.key.keysym.sym == SDLK_f )
                {
                    shadding == 'f' ? shadding='x' : shadding = 'f';
                    setShadding(shadding);
                    erase(true);
                    base.draw();
                    for(int i=0;i<n;i++)
                    {
                        s[i]->draw();
                    }
                    draw();
                }


                if( event.key.keysym.sym == SDLK_r )
                {

                        if (!neg)
                    angle+=2;
                    else
                    angle-=2;

                    if(angle > 10)
                    angle=10;
                    else if(angle < -10)
                    angle=-10;


                    erase(true);

                    base.rotate(angle); //axis

                    for(int i=0;i<n;i++)
                    {
                        s[i]->rotate(angle);
                    }
                    draw();
                }


                if( event.key.keysym.sym == SDLK_LEFT)
                {


                    if(anglez < -20)
                    anglez=-20;
                    else
                    anglez-=5;

                    erase(true);

                    base.rotate(anglez,'z'); //axis

                    //base.draw();
                    for(int i=0;i<n;i++)
                    {
                        s[i]->rotate(anglez,'z');
                    }
                    draw();
                }

                if( event.key.keysym.sym == SDLK_RIGHT)
                {

                    if(anglez > 20)
                    anglez=20;
                    else
                    anglez+=5;

                    erase(true);
                    base.rotate(anglez,'z'); //axis

                    for(int i=0;i<n;i++)
                    {
                        s[i]->rotate(anglez,'z');
                    }

                    draw();
                }

                if( event.key.keysym.sym == SDLK_UP)
                {
                    if(anglex < -20)
                    anglex=-20;
                    else
                    anglex-=5;
                    erase(true);
                    base.rotate(anglex,'x'); //axis
                    //base.draw();
                    for(int i=0;i<n;i++)
                    {
                        s[i]->rotate(anglex,'x'); //axis
                    }
                    draw();
                }

                if( event.key.keysym.sym == SDLK_DOWN)
                {
                    if(anglex > 30)
                    anglex=30;
                    else
                    anglex+=5;

                    erase(true);
                    base.rotate(anglex,'x'); //axis

                    //base.draw();
                    for(int i=0;i<n;i++)
                    {
                        s[i]->rotate(anglex,'x'); //axis
                    }
                    draw();
                }


                if( event.key.keysym.sym == SDLK_x )
                {
                    erase(true);
                    if(!neg){
                    base.translate('x','+',10);
                    for(int i=0;i<n;i++)
                    {
                        s[i]->translate('x','+',10);
                    }
                    }
                    else
                    {
                            base.translate('x','-',10);
                            for(int i=0;i<n;i++)
                    {
                        s[i]->translate('x','-',10);
                    }
                    }
//                    base.draw();
//                    for(int i=0;i<n;i++)
//                    {
//                        s[i]->draw();
//                    }
                    draw();
                }


                if( event.key.keysym.sym == SDLK_z )
                {
                    erase(true);
                    if(!neg)
                    {
                        base.translate('z','+',10);
                        for(int i=0;i<n;i++)
                    {
                        s[i]->translate('z','+',10);
                    }
                    }
                    else
                    {
                        base.translate('z','-',10);
                        for(int i=0;i<n;i++)
                    {
                        s[i]->translate('z','-',10);
                    }
                    }

//                    base.draw();
//                        for(int i=0;i<n;i++)
//                        {
//                            s[i]->draw();
//                        }
                    draw();
                }

            }

        }




  SDL_Flip(screen);

    }

    SDL_Quit();
    return 0;
}

