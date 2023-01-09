#ifndef LIFE_H
#define LIFE_H
#define CELL_SIZE 10 //10*10px
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define CANVAS_WIDTH  4*SCREEN_WIDTH
#define CANVAS_HEIGHT 4*SCREEN_HEIGHT
#include "SFML/Graphics.hpp"
#include<random>
#include<chrono>
#include<sstream>
#include<iostream>
class Life
{

    public:
        struct Cell{

              bool alive = false;
              sf::Vertex *topLeft;
              sf::Vertex *topRight;
              sf::Vertex *bottomLeft;
              sf::Vertex *bottomRight;
         Cell(){
         }
         void toggleState(){

             alive = !alive;
        }
        void setColor(const sf::Color& tl,const sf::Color& tr, const sf::Color& br,const sf::Color& bl){

             topLeft->color      = tl;
             topRight->color     = tr;
             bottomRight->color  = br;
             bottomLeft->color   = bl;
        }

        };
    public:

        Life();
        void startSim();
        void input(const sf::Event&);
        void reset();
        int uniformDistribution(int , int);
        ~Life();


    private:
        int generation = 0;
        int population = 0;
        bool right     = false;
        bool left      = false;
        bool up        = false;
        bool down      = false;
        bool leftClick = false;
        bool pauseUpdate = true;
        const int col;
        const int row;
        const int totalCells;
        bool *updateLife;
        sf::Text info;
        sf::Font font;
        sf::View cellView;
        sf::RenderWindow window;
        sf::VertexArray pixels;
        Cell* cellInfo;

};

#endif // LIFE_H
