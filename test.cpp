#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int sumReCar = 8;
const std::string link_ReCar[sumReCar] = {"cars/ReCar1.png", "cars/ReCar2.png", "cars/ReCar3.png",
                                        "cars/ReCar4.png", "cars/ReCar5.png", "cars/ReCar6.png",
                                        "cars/ReCar7.png", "cars/ReCar8.png"};
int Score = 0;

//Loads individual image as texture

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;
SDL_Texture* gTextureCar = NULL;
SDL_Texture* gTextureBackground = new SDL_Texture[2];
gTextureBackground[0]  = NULL;
gTextureBackground[1]  = NULL;
SDL_Texture* gTextureReverseCar[sumReCar] ;

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool init()
{
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Avoid Accidents (Alpha)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    gTextureBackground[0] = loadTexture("background/intro_background.jpg");
    gTextureBackground[1] = loadTexture("background/intro_background2.jpg");
    if( gTextureBackground[0] == NULL ||gTextureBackground[1] == NULL)
    {
        printf( "Failed to load background!\n" );
        success = false;
    }
    gTexture = loadTexture( "background/main_road.png" );
    if( gTexture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }

    gTextureCar = loadTexture("cars/car.png");
    if( gTextureCar == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }
    for (int i=0; i<sumReCar; ++i)
    {
        gTextureReverseCar[i] = loadTexture(link_ReCar[i]);
        if( gTextureReverseCar[i] == NULL)
        {
            printf( "Failed to load texture image!\n" );
            success = false;
        }
    }
    return success;
}



int main( int argc, char* args[] )
{
    int countIMG,varC=1;
    if (!init())
    {
        printf("Unable to create Window");
    }
        else
    {
        if (!loadMedia())
        {
            printf("Unable to load Media");
        }
            else
        {
            bool quit = false;

			//Event handler
			SDL_Event e;
			SDL_Rect RR_Car = {620, 610, 60, 110}; // initial position of player's car
			SDL_Rect RR_ReverseCar = {0, 0, 60, 100}; // cars appearing from the reverse direction

			//While application is running
            int i = 0, speed = 8, outBG = 0;
            bool appear = false;
            int flag;
			while( !quit )
			{
                if (!appear)
                {
                    RR_ReverseCar.x = 425 + rand()%350;
                    RR_ReverseCar.y = 0;
                    appear = true;
                    flag = rand()%8;
                    speed += 3;
                    i = 0;
                }
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					if ( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_LEFT:
                            RR_Car.x = RR_Car.x - 30;
                            break;
                            case SDLK_RIGHT:
                            RR_Car.x = RR_Car.x + 30;
                            break;
                            case SDLK_RETURN:
                            ++outBG;
                            break;
                        }
                    }
				}
				RR_Car.x = std::min(RR_Car.x, 435+350);
				RR_Car.x = std::max(RR_Car.x, 435);
                countIMG = varC % 2;
                varC ++;
				if (outBG == 0) // display intro image
                {

                    SDL_RenderCopy(gRenderer, gTextureBackground[countIMG], NULL, NULL);
                    SDL_RenderPresent(gRenderer);
                }
                else
                {

                    RR_ReverseCar.y = i;
                    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                    SDL_RenderCopy(gRenderer, gTextureCar , NULL, &RR_Car);
                    SDL_RenderCopy(gRenderer, gTextureReverseCar[flag], NULL, &RR_ReverseCar);
                    SDL_RenderPresent(gRenderer);
                    SDL_Delay(100);
                    i += speed;

                    SDL_Rect RR_result ; // check collision
                    if (SDL_IntersectRect(&RR_Car, &RR_ReverseCar, &RR_result))
                    {
                        quit = true;
                    }
                    if (RR_ReverseCar.y >= 720)
                    {
                        appear = false;
                    }
                }
			}
        }
    }
    return 0;
}
