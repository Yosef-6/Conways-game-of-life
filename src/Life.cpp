#include "Life.h"

  Life::Life():window(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT),"Conway's game of Life (vYOSEF)",sf::Style::Fullscreen|sf::Style::Close)
 ,pixels(sf::Quads),col(CANVAS_WIDTH/CELL_SIZE),row(CANVAS_HEIGHT/CELL_SIZE)
 ,totalCells((CANVAS_WIDTH/CELL_SIZE)*(CANVAS_HEIGHT/CELL_SIZE))
 ,cellView(sf::Vector2f(SCREEN_WIDTH/2.f,SCREEN_HEIGHT/2.f),sf::Vector2f(SCREEN_WIDTH,SCREEN_HEIGHT))
{
     font.loadFromFile("Fonts/arial.ttf");
     info.setFont(font);
     info.setPosition(30,50);
     info.setCharacterSize(16);
     window.setFramerateLimit(20);
     cellInfo = new Cell[totalCells];
     updateLife = new bool[totalCells];
   for(int i = 0;i<totalCells;i++){

      pixels.append(sf::Vertex(sf::Vector2f(((i%col)*CELL_SIZE),(i/col)*CELL_SIZE),sf::Color(169,169,169)));
      pixels.append(sf::Vertex(sf::Vector2f(((i%col)*CELL_SIZE + CELL_SIZE),(i/col)*CELL_SIZE),sf::Color(169,169,169)));
      pixels.append(sf::Vertex(sf::Vector2f(((i%col)*CELL_SIZE + CELL_SIZE),(i/col)*CELL_SIZE + CELL_SIZE),sf::Color(169,169,169)));
      pixels.append(sf::Vertex(sf::Vector2f(((i%col)*CELL_SIZE),(i/col)*CELL_SIZE + CELL_SIZE),sf::Color(169,16,169)));

   }

   int pixel = 0;
   for(int i = 0;i<totalCells; i++)
   {
       cellInfo[i].topLeft    = &pixels[pixel++];
       cellInfo[i].topRight   = &pixels[pixel++];
       cellInfo[i].bottomRight= &pixels[pixel++];
       cellInfo[i].bottomLeft = &pixels[pixel++];
       updateLife[i] = false;
   }

   cellView.setCenter(CANVAS_WIDTH/2.f,CANVAS_HEIGHT/2.f);
}
void Life::startSim(){

    std::stringstream s;
    std::string fp , gen , pop;
    sf::Clock clock;
    while(window.isOpen())
    {
            float elapsedTime = clock.restart().asSeconds();
            s<< 1.0f/elapsedTime;
            s>>fp;
            s.clear();
            s<<generation;
            s>>gen;
            s.clear();
            s<<population;
            s>>pop;
            s.clear();
            info.setString("FPS : " + fp + " \nPOPULATION :" + pop + "\nGENERATION :" + gen);


            sf::Event e;
            while(window.pollEvent(e)){
            if(e.type == sf::Event::MouseWheelMoved)
            cellView.zoom(1.f+e.mouseWheel.delta*0.1f);
            input(e); //key pressed and mouse pressed events
            }

            { //input scope
            if(up == true)
            cellView.move(0,-10.f);
            if(left == true)
            cellView.move(-10.f,0);
            if(right == true)
            cellView.move(10.f,0);
            if(down == true)
            cellView.move(0,10.f);
            if(leftClick == true)
            {
                int x = window.mapPixelToCoords(sf::Mouse::getPosition(window),cellView).x/CELL_SIZE;
                int y = window.mapPixelToCoords(sf::Mouse::getPosition(window),cellView).y/CELL_SIZE;
                if(!cellInfo[y*col + x].alive)
                {
                    cellInfo[y*col + x].toggleState();
                    cellInfo[y*col + x].setColor(sf::Color::Black,sf::Color::Black,sf::Color::Black,sf::Color::Black);
                    updateLife[y*col + x] = true;
                }

            }

            } //end of input scope

            if(!pauseUpdate)
            { // conway's rules /
                population = 0;
                auto checkIndex = [this](int index){

                             if(cellInfo[index].alive == true)
                                return true;
                             else
                                return false;
                };
                  for(int i = 0;i<totalCells; i++)
                  {

                      int liveNeighbours = 0;
                      ((i+1)%col == 0) ? (checkIndex(i + 1 - col) == true ? liveNeighbours++ : 0) :  (checkIndex(i+1) == true ?  liveNeighbours++ : 0 );                //rightNeighbour
                      ((i)%col == 0) ? (checkIndex(i + col -1) == true ? liveNeighbours++ : 0) :  (checkIndex(i-1) == true ?  liveNeighbours++ : 0 );                  //leftNeighbour
                      ((i-col) < 0) ? (checkIndex((row - 1)*col + i) == true ? liveNeighbours++ : 0) :  (checkIndex(i-col) == true ?  liveNeighbours++ : 0);            //topNeighbour
                      ((i+col) > totalCells-1) ? (checkIndex(i - (row - 1)*col) == true ? liveNeighbours++ : 0) :  (checkIndex(i+col) == true ?  liveNeighbours++ : 0 ); //DownNeighbour
                      ((i-col) < 0 && (row - 1)*col + i + 1 > totalCells - 1) ? (checkIndex((row - 1)*col + i + 1 - col) == true ? liveNeighbours++ : 0) : ((i - col  <  0) ? (checkIndex((row - 1)*col + i + 1) ? liveNeighbours++ : 0 ) :(checkIndex(i - col + 1) ? liveNeighbours++ : 0 )); // topRight
                      ((i-col) < 0 && ((row - 1)*col + i )%col == 0) ? (checkIndex((row - 1)*col + i + col - 1) == true ? liveNeighbours++ : 0) : ((i - col  <  0) ? (checkIndex((row - 1)*col + i - 1) ? liveNeighbours++ : 0 ) :(checkIndex(i - col - 1) ? liveNeighbours++ : 0 )); // topLeft
                      ((i+col) > totalCells - 1 && (((i - (row - 1)*col)+ 1) % col == 0)) ? (checkIndex((i - (row - 1)*col) + 1 - col) == true ? liveNeighbours++ : 0) : ((i + col > totalCells - 1) ? (checkIndex(i - (row - 1)*col + 1) ? liveNeighbours++ : 0 ) :(checkIndex(i + col + 1) ? liveNeighbours++ : 0 )); // bottomRight
                      ((i+col) > totalCells - 1 && ( (i - (row - 1)*col)%col == 0 )) ? (checkIndex((i - (row - 1)*col) - 1 + col) == true ? liveNeighbours++ : 0) : ((i + col > totalCells - 1) ? (checkIndex(i - (row - 1)*col - 1) ? liveNeighbours++ : 0 ) :(checkIndex(i + col - 1) ? liveNeighbours++ : 0 )); // bottomLeft

                      if(checkIndex(i) == true){
                           population++;
                           if(liveNeighbours < 2 || liveNeighbours >3)
                           updateLife[i] = false;
                           else
                           updateLife[i] = true;
                      }
                      else
                           if(liveNeighbours == 3)
                           updateLife[i] = true;

                   }

                   //setState
                   for(int i = 0;i<totalCells;i++)
                   {

                           if(updateLife[i] != checkIndex(i)){

                                  cellInfo[i].toggleState();
                                  if(updateLife[i])
                                  cellInfo[i].setColor(sf::Color::Black,sf::Color::Black,sf::Color::Black,sf::Color::Black);
                                  else
                                  cellInfo[i].setColor(sf::Color(169,169,169),sf::Color(169,169,169),sf::Color(169,169,169),sf::Color(169,169,169));
                                  }
                    }


            generation++;
         }    //end of conway's rules



            window.clear();
            window.setView(cellView);
            window.draw(pixels);
            window.setView(window.getDefaultView());
            window.draw(info);
            window.display();



    }




}
void Life::input(const sf::Event& e)
{
    if(e.type == sf::Event::KeyPressed)
    {
        switch(e.key.code){

        case  sf::Keyboard::Right :
        right  =true;
        break;
        case  sf::Keyboard::Left :
        left   =true;
        break;
        case  sf::Keyboard::Up :
        up     =true;
        break;
        case  sf::Keyboard::Down :
        down   =true;
        break;
        case  sf::Keyboard::Space :
        pauseUpdate = false;
        break;
        case  sf::Keyboard::R :
        reset();
        break;

        default:
        break;
    }

    }
    if(e.type == sf::Event::KeyReleased)
    {
        switch(e.key.code){

        case sf::Keyboard::Right :
        right  =false;
        break;
        case  sf::Keyboard::Left :
        left   =false;
        break;
        case  sf::Keyboard::Up :
        up     =false;
        break;
        case  sf::Keyboard::Down :
        down   =false;
        break;
        default:
        break;
    }

    }
    if(e.type == sf::Event::MouseButtonPressed)
    {

    switch(e.mouseButton.button){
        case sf::Mouse::Left:
        leftClick = true;
        break;
        default:
        break;

    }

    }
    if(e.type == sf::Event::MouseButtonReleased)
    {
        switch(e.mouseButton.button){
        case sf::Mouse::Left:
        leftClick = false;
        break;
        default:
        break;

    }


    }

}

int Life::uniformDistribution(int min , int max){
    static std::default_random_engine defaultRandom (std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(defaultRandom);
}
void Life::reset(){

      for(int i = 0; i<totalCells;i++)
          {
             updateLife[i] = false;
             cellInfo[i].alive = false;
             cellInfo[i].setColor(sf::Color(169,169,169),sf::Color(169,169,169),sf::Color(169,169,169),sf::Color(169,16,169));
          }
          generation = 0;
          population = 0;
          pauseUpdate = true;
}
Life::~Life()
{
    pixels.clear();
    delete []cellInfo;
    delete []updateLife;
}
