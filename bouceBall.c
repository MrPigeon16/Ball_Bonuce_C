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
#define FPS 60
#define TARGET_FPS (1000 / FPS)
#define G_FORCE 0.2
#define E_MATERIAL 0.1
#define ENERGY(arg) ( (arg) * (arg) / 2.0 * G_FORCE )
#define X_RES 1.5

int window = 1;
int last_frame_time = 0;



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

    SDL_Log("x speed - %f", ball->vel_x);
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
    
        SDL_Log("center - r = %f", ball->y_center - ball->r);
        if(ball->vel_y < 0)
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
    SDL_Delay(20);

}


void render(SDL_Renderer* renderer, Circle* ball)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_ball(renderer, *ball);
    SDL_RenderPresent(renderer);
    

}

int main(void)
{

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

    //Circle* ball = (Circle*)malloc(sizeof(Circle)*1);
    //Circle ball = {250,250,50};
    //ball->y_center = 250;
    //ball->x_center = 250;
    //ball->r =50;
    
    Circle ball = (Circle){200,200,100,20,10,1,1};

    draw_ball(renderer, ball);
    SDL_RenderPresent(renderer);



    while(window)
    { 
        handle_input();
        bounce_ball(renderer, &ball);
        render(renderer, &ball);
        SDL_Delay(10);
    }
  
    printf("I bout to bounce!\n");



    return 0;
}



void render_obj(SDL_Renderer* renderer, Circle* c)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_ball(renderer, *c);
    
    SDL_RenderPresent(renderer);
    

}
void fall(SDL_Renderer* renderer, Circle* ball)
{

    
    float deltaTime = (SDL_GetTicks() - last_frame_time) / 1000.0;
    last_frame_time = SDL_GetTicks();
    
    if(ball->y_center >= 0 && ball->y_center + ball->r <= HEIGTH &&
    ball->x_center >= 0 && ball->x_center + ball->r <= WIDTH)
    {
        ball->y_center += 200 * deltaTime;
        SDL_Log("Hello!!!");
    }
    else
    {
        ball->y_center -= 200 * deltaTime;
        SDL_Log("Hello");
    }
}

