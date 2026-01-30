#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>


#define COLOR_WHITE 0xffffffff

#define WIDTH 900
#define HEIGTH 600
//#define FPS 60 // not in use
//#define TARGET_FPS (1000 / FPS) // not in use
#define G_FORCE 0.2
#define E_MATERIAL 0.1
#define X_RES 1.5

int window = 1;
//int last_frame_time = 0; // not in use



typedef struct
{
    double x_center;
    double y_center;
    double r;
    double vel_x;
    double vel_y;
    int flag_y;
    int flag_x;
}Circle;



void draw_ball(SDL_Renderer* renderer, Circle circle)
{
    for(double y = -(circle.r); y <= circle.r; y++)
    {
        for(double x = -(circle.r) ;x <= circle.r; x++)
        {
            if(x * x + y * y <= circle.r * circle.r)
            {
                SDL_RenderDrawPoint(renderer, circle.x_center + x, circle.y_center + y);
            }
        }
    } 
}



void handle_input()
{
    SDL_Event e;
    //close the window
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
                window = 0;
        }    
    }

}



void bounce_ball(SDL_Renderer* renderer, Circle* ball)
{

    if(ball->flag_y)
    {  

        ball->y_center += ball->vel_y;
        ball->vel_y += G_FORCE;
        if(ball->y_center + ball->r >= HEIGTH)
        {
            ball->flag_y = 0;
        }
    }
   
    if(ball->flag_x)
    {  
        ball->x_center += ball->vel_x;
        if(ball->x_center + ball->r >= WIDTH)
        {
            ball->flag_x = 0;
            ball->vel_x -= X_RES;
        }
    }
    if(!(ball->flag_y)) 
    {
        ball->y_center -= ball->vel_y;
        ball->vel_y -= G_FORCE + E_MATERIAL;
    
        if(ball->vel_y < 0 || ball->y_center - ball->r <= 0)
        { 
            ball->flag_y = 1;   
        }
    }

    if(!(ball->flag_x)) 
    {
        ball->x_center -= ball->vel_x;
    
        if(ball->x_center - ball->r <= 0)
        { 
            ball->flag_x = 1;
            ball->vel_x -= X_RES;
        }
    }

}


void render(SDL_Renderer* renderer, Circle* ball)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_ball(renderer, *ball);
    SDL_RenderPresent(renderer);
    
}

int main(int argc, char **argv)
{
    SDL_Log("\nball radius - %s\nball init y vel - %s\nball init x vel - %s\n",argv[1],argv[2],argv[3]);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* ball_window = SDL_CreateWindow(
        "BounceBall", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH, 
        HEIGTH,
        0);


    SDL_Renderer* renderer = SDL_CreateRenderer(ball_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
     
    Circle ball = (Circle){250,250,atoi(argv[1]),atof(argv[2]),atof(argv[3]),1,1};

    draw_ball(renderer, ball);
    SDL_RenderPresent(renderer);

    while(window)
    { 
        handle_input();
        bounce_ball(renderer, &ball);
        render(renderer, &ball);
        SDL_Delay(20);
    }
  
    printf("I bout to bounce\n");
    return 0;
}


