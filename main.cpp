#include <SDL.h>
#include <SDL_image.h>
#include <bits/stdc++.h>
#include <SDL_ttf.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int sumReCar = 8;
const std::string link_ReCar[sumReCar] = {"cars/ReCar1.png", "cars/ReCar2.png", "cars/ReCar3.png",
                                        "cars/ReCar4.png", "cars/ReCar5.png", "cars/ReCar6.png",
                                        "cars/ReCar7.png", "cars/ReCar8.png"};
int Score = 0;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;
SDL_Texture* gTextureCar = NULL;
SDL_Texture* gTextureBackground[2] = {NULL, NULL};
SDL_Texture* gTextureGameOverScreen = NULL;
SDL_Texture* gTextureReverseCar[sumReCar];
SDL_Texture* gTextureArrow = NULL;

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
		gWindow = SDL_CreateWindow( "Avoid Accidents", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

    gTextureBackground[0] = loadTexture("background/intro_background.png");
    gTextureBackground[1] = loadTexture("background/intro_background2.png");

    if( gTextureBackground[0] == NULL || gTextureBackground[1] == NULL )
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
    for (int i = 0; i < sumReCar; ++i)
    {
        gTextureReverseCar[i] = loadTexture(link_ReCar[i]);
        if( gTextureReverseCar[i] == NULL)
        {
            printf( "Failed to load texture image!\n" );
            success = false;
        }
    }
    gTextureGameOverScreen = loadTexture("background/GameOverScreen.png");
    if (gTextureGameOverScreen == NULL)
    {
        printf("Failed to load GameOver screen!");
        success = false;
    }
    gTextureArrow = loadTexture("background/arrow.png");
    if (gTextureArrow == NULL)
    {
        printf("Failed to load arrow!");
        success = false;
    }
    return success;
}



int main( int argc, char* args[] )
{
    system("cls");
    int score = 0;
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

            int i = 0, speed = 8, outBG = 0;

            bool appear = false;

            int flag;
			while( !quit )
			{
                if (!appear) // reverse cars appear and go ahead
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
                        }
                    }
				}
				RR_Car.x = min(RR_Car.x, 435+350);
				RR_Car.x = max(RR_Car.x, 435);

                int ThuTuInAnh;
                ThuTuInAnh = 0;

                while (outBG == 0)
                {
                    SDL_RenderCopy(gRenderer, gTextureBackground[ThuTuInAnh], NULL, NULL);
                    SDL_Delay(300);
                    SDL_RenderPresent(gRenderer);
                    system("cls");
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }

                        if ( e.type == SDL_KEYDOWN )
                        {
                            if (e.key.keysym.sym == SDLK_RETURN)
                            {
                                ++outBG;
                            }
                        }
                    }
                    if (ThuTuInAnh == 0)
                    {
                        ThuTuInAnh = 1;
                    }
                    else ThuTuInAnh = 0;
                }
                cout << "Score: " << score << endl;
                RR_ReverseCar.y = i;
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                SDL_RenderCopy(gRenderer, gTextureCar , NULL, &RR_Car);
                SDL_RenderCopy(gRenderer, gTextureReverseCar[flag], NULL, &RR_ReverseCar);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(100);
                system("cls");
                i += speed; // speed up

                SDL_Rect RR_result ; // check collision
                if (SDL_IntersectRect(&RR_Car, &RR_ReverseCar, &RR_result))
                {
                   outBG = 0;
                   cout << "Score: " << score <<  endl;
                   appear = false;
                   int flag = 0;
                   while (outBG == 0) // Game Over
                   {
                       SDL_Rect arrowPosition[2];
                       arrowPosition[0] = {432, 257, 85, 70}; // position of arrow 0
                       arrowPosition[1] = {432, 447, 85, 70}; // position of arrow 1
                       SDL_RenderCopy(gRenderer, gTextureGameOverScreen, NULL, NULL);
                       SDL_RenderCopy(gRenderer, gTextureArrow, NULL, &arrowPosition[flag]);
                       SDL_RenderPresent(gRenderer);
                       while( SDL_PollEvent( &e ) != 0 )
                       {
                            //User requests quit
                            if( e.type == SDL_QUIT )
                            {
                                quit = true;
                                ++outBG;
                            }
                            if ( e.type == SDL_KEYDOWN )
                            {
                                if (e.key.keysym.sym == SDLK_RETURN)
                                {
                                    if (flag == 0)
                                    {
                                        ++outBG;
                                        system("cls");
                                        speed = 8; // back to the initial speed
                                        score = 0;
                                    }
                                    if (flag == 1)
                                    {
                                        ++outBG;
                                        quit = true; // out game
                                        cout << "Total score: " << score << endl;
                                    }
                                }
                                if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP )
                                {
                                    flag = 1-flag;
                                }
                            }
                       }
                   }
                }
                if (RR_ReverseCar.y >= 720)
                {
                    appear = false; // clear reverse cars
                    score += 5;
                    //cout << "Score: " << score <<  endl;
                }
            }
        }
    }
    return 0;
}
