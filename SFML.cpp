#include <iostream>
#include "sfml/Graphics.hpp"
#include <random>
#include <fstream>
#include <sstream>
using namespace std;
using namespace sf;
//Variables*
int board[5][5];
int linescount = 0;
const int boardSize = 5;
const int radius = 30;
const int tileSize = 80;
int result;
int kill1 = 0, kill2 = 0;
Font font;
Text text;
bool isValid = false, menu = true, showcont = false, won = false;
bool reading = false, invalid = false, start = true, tossdone = true;
Vector2i selectedBeadCoords(-1, -1); // MULHAIZA CLICK WALI BEAD
Vector2i BeadCoords(-1, -1);
stringstream print3;
int mouseX;
int mouseY;
int linenum = 0;
int key = 0;

//FUNCTIONS

//TASS HO RHA
int toss() {
    int num;
    srand(time(0));
    num = rand();
    return (num % 2);
}


//intialising the board
void intialise() {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            //p1 = 1 , p2 = 2, empty = 0
            //first 3 rows intialize
            if (i < 3) {
                board[i][j] = 1;
                if (i == 2 && j < 2) {
                    board[i][j] = 1;//Player 1 beads initalise
                }
                else if (i == 2 && j > 2) {
                    board[i][j] = 2;//initalise player 2
                }
                else if (i == 2)
                    board[i][j] = 0;
            }
            else if (i > 2) {
                board[i][j] = 2;
            }
        }
    kill1 = 0;
    kill2 = 0;
    result = toss() + 1;
}

//line gin rha
void countline() {
    ifstream read;
    string number;
    linescount = 0;
    read.open("savefiles.txt");
    if (read.peek() == std::ifstream::traits_type::eof() == true)
    {
        linescount = 0;
    }
    else {
        while (!read.eof()) {
            getline(read, number);
            linescount++;
        }
    }
    read.close();
}

//Writing in file after game
void filewrite() {
    countline();
    ofstream MyFile("savefiles.txt", ios::app);
    if (linescount != 0)
        MyFile << "\n";
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            MyFile << board[i][j];
            MyFile << " ";
        }
    }
    MyFile << result;
    MyFile << " ";
    MyFile << kill1;
    MyFile << " ";
    MyFile << kill2;
    MyFile.close();
}


//JEETNAY WALI SURETHAAL
bool checkWin(int board[][5], int player) {
    // ya code dekhta k kon jeeta
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] == player) {
                return false; // Gotiyan abhi bhi hain
            }
        }
    }
    return true; // Koi goti nhi ha
}


//RANG MIL RHAY
sf::Color getCircleColor(int i, int j) {
    if (board[i][j] == 1) {
        return sf::Color(255, 0, 0);  // Halkay RANG WALA GOL DAIRA
    }
    else {
        if (board[i][j] == 2) {
            return sf::Color(0, 220, 0);   //  TEIZ RANG WALA GOL DAIRA
        }
    }
}

//GOOL DIARA BAN RHA HA
sf::CircleShape createCircle(int i, int j) {
    sf::CircleShape Circle;
    Circle.setRadius(radius);
    Circle.setPosition((j * radius * 4) + 4, (i * radius * 4) + 4);
    Circle.setFillColor(getCircleColor(i, j));
    if (i == selectedBeadCoords.y && j == selectedBeadCoords.x && isValid == true) {
        Circle.setOutlineThickness(4.6);
        Circle.setOutlineColor(sf::Color::Yellow);
    }
    return Circle;
}


//DABAY BANA RHA
void drawBoard(sf::RenderWindow& window) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] != 0) {
                sf::CircleShape Circle = createCircle(i, j);
                window.draw(Circle);

            }
        }
    }
}

//LAKIRAY BAN RHI HAIN
void drawLines(RenderWindow& window) {
    Vertex line3[] = { Vertex(Vector2f(30.f,510.f)),Vertex(Vector2f(510.f,30.f)) };
    window.draw(line3, 2, Lines);
    Vertex line4[] = { Vertex(Vector2f(30.f,30.f)),Vertex(Vector2f(510.f,510.f)) };
    window.draw(line4, 2, Lines);
    Vertex line5[] = { Vertex(Vector2f(270.f,30.f)),Vertex(Vector2f(30.f,270.f)) };
    window.draw(line5, 2, Lines);
    Vertex line6[] = { Vertex(Vector2f(30.f,270.f)),Vertex(Vector2f(270.f,510.f)) };
    window.draw(line6, 2, Lines);
    Vertex line7[] = { Vertex(Vector2f(510.f,270.f)),Vertex(Vector2f(270.f,510.f)) };
    window.draw(line7, 2, Lines);
    Vertex line8[] = { Vertex(Vector2f(510.f,270.f)),Vertex(Vector2f(270.f,30.f)) };
    window.draw(line8, 2, Lines);


    for (int i = 0; i < boardSize; ++i) {
        Vertex line1[] = { Vertex(Vector2f(30.f,i * 120 + 30.f)),Vertex(Vector2f(30 + 30 * 4 * 4.f,i * 120 + 30.f)) };
        window.draw(line1, 2, Lines);
    }
    for (int i = 0; i < boardSize; ++i) {
        Vertex line2[] = { Vertex(Vector2f(i * 120 + 30.f,30.f)),Vertex(Vector2f(i * 120 + 30.f,30 + 30 * 4 * 4.f)) };
        window.draw(line2, 2, Lines);
    }
}

//GOTI CHECK KRNA K SAHI HA KA NAI
bool valid(int board[][5], int x, int y, int turn) {
    if (board[y][x] == turn && x <= 4 && x >= 0 && y <= 4 && y >= 0) { // changes made by ahmad
        return true;
    }
    else
        return false;

}

//SARAY possible moves
void possibilities(int board[][5], int& x_orginal, int& y_original, int turn, RenderWindow& window) {
    bool invalid = true, diagnol = false;//For diagnols
    int opponent = 1;
    if (turn == 1)
        opponent = 2;
    sf::CircleShape Circle;
    Circle.setRadius(radius);
    Circle.setFillColor(Color::Transparent);

    Circle.setOutlineThickness(-5);
    Circle.setOutlineColor(sf::Color::Cyan);
    if ((x_orginal % 2 == 0 && y_original % 2 == 0) || (x_orginal % 2 == 1 && y_original % 2 == 1))
        diagnol = true;

    if (board[x_orginal + 1][y_original] == 0 && x_orginal + 1 <= 4) {// if there is an empty  space beside the original coordinate
        Circle.setPosition((y_original)*radius * 4, (x_orginal + 1) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal][y_original - 1] == 0 && y_original - 1 >= 0) {
        Circle.setPosition((y_original - 1) * radius * 4, (x_orginal)*radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal - 1][y_original] == 0 && x_orginal - 1 >= 0) {
        Circle.setPosition((y_original)*radius * 4, (x_orginal - 1) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal][y_original + 1] == 0 && y_original + 1 <= 4) {
        Circle.setPosition((y_original + 1) * radius * 4, (x_orginal)*radius * 4);
        window.draw(Circle);
    }
    //Diagnols Condition

    if (board[x_orginal - 1][y_original - 1] == 0 && diagnol == true && y_original - 1 >= 0 && x_orginal - 1 >= 0) {
        Circle.setPosition((y_original - 1) * radius * 4, (x_orginal - 1) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal + 1][y_original - 1] == 0 && diagnol == true && x_orginal + 1 <= 4 && y_original - 1 >= 0) {
        Circle.setPosition((y_original - 1) * radius * 4, (x_orginal + 1) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal - 1][y_original + 1] == 0 && diagnol == true && y_original + 1 <= 4 && x_orginal - 1 >= 0) {
        Circle.setPosition((y_original + 1) * radius * 4, (x_orginal - 1) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal + 1][y_original + 1] == 0 && diagnol == true && x_orginal + 1 <= 4 && y_original + 1 <= 4) {
        Circle.setPosition((y_original + 1) * radius * 4, (x_orginal + 1) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal + 2][y_original - 2] == 0 && board[x_orginal + 1][y_original - 1] == opponent && diagnol == true && x_orginal + 2 <= 4 && y_original - 2 >= 0) {  // in case we want to kill
        Circle.setPosition((y_original - 2) * radius * 4, (x_orginal + 2) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal + 2][y_original] == 0 && board[x_orginal + 1][y_original] == opponent && x_orginal + 2 <= 4) {
        Circle.setPosition((y_original)*radius * 4, (x_orginal + 2) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal][y_original - 2] == 0 && board[x_orginal][y_original - 1] == opponent && y_original - 2 >= 0) {
        Circle.setPosition((y_original - 2) * radius * 4, (x_orginal)*radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal - 2][y_original - 2] == 0 && board[x_orginal - 1][y_original - 1] == opponent && diagnol == true && y_original - 2 >= 0 && x_orginal - 2 >= 0) {
        Circle.setPosition((y_original - 2) * radius * 4, (x_orginal - 2) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal - 2][y_original] == 0 && board[x_orginal - 1][y_original] == opponent && x_orginal - 2 >= 0) {
        Circle.setPosition((y_original)*radius * 4, (x_orginal - 2) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal - 2][y_original + 2] == 0 && board[x_orginal - 1][y_original + 1] == opponent && diagnol == true && y_original + 2 <= 4 && x_orginal - 2 >= 0) {
        Circle.setPosition((y_original + 2) * radius * 4, (x_orginal - 2) * radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal][y_original + 2] == 0 && board[x_orginal][y_original + 1] == opponent && y_original + 2 <= 4) {
        Circle.setPosition((y_original + 2) * radius * 4, (x_orginal)*radius * 4);
        window.draw(Circle);
    }
    if (board[x_orginal + 2][y_original + 2] == 0 && board[x_orginal + 1][y_original + 1] == opponent && diagnol == true && x_orginal + 2 <= 4 && y_original + 2 <= 4) {
        Circle.setPosition((y_original + 2) * radius * 4, (x_orginal + 2) * radius * 4);
        window.draw(Circle);
    }
}

//GOTI Ki IKHTATAM JAGA
bool check(int board[][5], int& X_final, int& Y_final, int& x_orginal, int& y_original, int turn) {
    bool invalid = true, diagnol = false;//For diagnols
    int opponent = 1;
    if (turn == 1)
        opponent = 2;


    invalid = true;
    if ((x_orginal % 2 == 0 && y_original % 2 == 0) || (x_orginal % 2 == 1 && y_original % 2 == 1)) {
        diagnol = true;
    }
    if (board[X_final][Y_final] != 0)
        return true;

    else if (X_final < 0 || X_final > 4 || Y_final < 0 || Y_final > 4) //Out of board
        return true;

    else {
        if (x_orginal + 1 == X_final && y_original == Y_final);// if there is an empty  space beside the original coordinate
        else if (x_orginal == X_final && y_original - 1 == Y_final);
        else if (x_orginal - 1 == X_final && y_original == Y_final);
        else if (x_orginal == X_final && y_original + 1 == Y_final);
        //Diagnols Condition

        else if (x_orginal - 1 == X_final && y_original - 1 == Y_final && diagnol == true);
        else if (x_orginal + 1 == X_final && y_original - 1 == Y_final && diagnol == true);
        else if (x_orginal - 1 == X_final && y_original + 1 == Y_final && diagnol == true);
        else if (x_orginal + 1 == X_final && y_original + 1 == Y_final && diagnol == true);

        else if (x_orginal + 2 == X_final && y_original - 2 == Y_final && board[x_orginal + 1][y_original - 1] == opponent && diagnol == true)  // in case we want to kill
            board[x_orginal + 1][y_original - 1] = 0;
        else if (x_orginal + 2 == X_final && y_original == Y_final && board[x_orginal + 1][y_original] == opponent)
            board[x_orginal + 1][y_original] = 0;
        else if (x_orginal == X_final && y_original - 2 == Y_final && board[x_orginal][y_original - 1] == opponent)
            board[x_orginal][y_original - 1] = 0;
        else if (x_orginal - 2 == X_final && y_original - 2 == Y_final && board[x_orginal - 1][y_original - 1] == opponent && diagnol == true)
            board[x_orginal - 1][y_original - 1] = 0;
        else if (x_orginal - 2 == X_final && y_original == Y_final && board[x_orginal - 1][y_original] == opponent)
            board[x_orginal - 1][y_original] = 0;
        else if (x_orginal - 2 == X_final && y_original + 2 == Y_final && board[x_orginal - 1][y_original + 1] == opponent && diagnol == true)
            board[x_orginal - 1][y_original + 1] = 0;
        else if (x_orginal == X_final && y_original + 2 == Y_final && board[x_orginal][y_original + 1] == opponent)
            board[x_orginal][y_original + 1] = 0;
        else if (x_orginal + 2 == X_final && y_original + 2 == Y_final && board[x_orginal + 1][y_original + 1] == opponent && diagnol == true)
            board[x_orginal + 1][y_original + 1] = 0;
        else
            return true;
    }
    return false;
}

//Reading array data from file
void fileread() {
    ifstream in;
    int count = 1;
    string numbers, temp;
    if (linescount != 0) {
        in.open("savefiles.txt");
        while (!in.eof() && count < linenum) {
            getline(in, temp);
            count++;
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                in >> board[i][j];
            }
        }

        in >> result;
        in >> kill1;
        in >> kill2;
        in.close();
    }
}

//TEXT display karna
void prntmenu(RenderWindow& window) {

    // Create rectangles for each menu option
    RectangleShape continueRect(Vector2f(225.f, 50.f));
    continueRect.setPosition(325.f, 35.f);
    continueRect.setFillColor(Color::Black);
    continueRect.setOutlineColor(Color::White);
    continueRect.setOutlineThickness(2.f);

    RectangleShape newGameRect(Vector2f(245.f, 50.f));
    newGameRect.setPosition(325.f, 175.f);
    newGameRect.setFillColor(Color::Black);
    newGameRect.setOutlineColor(Color::White);
    newGameRect.setOutlineThickness(2.f);

    RectangleShape loadGameRect(Vector2f(263.f, 50.f));
    loadGameRect.setPosition(325.f, 315.f);
    loadGameRect.setFillColor(Color::Black);
    loadGameRect.setOutlineColor(Color::White);
    loadGameRect.setOutlineThickness(2.f);

    RectangleShape exitRect(Vector2f(100, 55.f));
    exitRect.setPosition(325.f, 455.f);
    exitRect.setFillColor(Color::Black);
    exitRect.setOutlineColor(Color::White);
    exitRect.setOutlineThickness(2.f);
    // Draw rectangles

    window.draw(newGameRect);
    window.draw(loadGameRect);
    window.draw(exitRect);

    stringstream print, print2, print1;
    if (showcont == false) {
        font.loadFromFile("Fonts/PIXEAB__.ttf");
        print1 << "12 Bead's Game";
        text.setPosition(120.f, 35.f);
        text.setString(print1.str());
        text.setCharacterSize(60);
        text.setFillColor(sf::Color(250, 255, 250));
        window.draw(text);
        text.setCharacterSize(55);
        text.setFillColor(sf::Color(255, 255, 255));
        font.loadFromFile("Fonts/Dosis-Light.ttf");
    }
    if (showcont == true) {
        print << "CONTINUE\n\n";
        window.draw(continueRect);
    }
    else
        print << "\n\n";
    print << "NEW GAME \n\n"
        << "LOAD GAME \n\n"
        << "EXIT\n\n";
    text.setPosition(325.f, 25.f);
    text.setString(print.str());
    window.draw(text);

}

void display(RenderWindow& window) {
    stringstream print, print2;
    if (tossdone == false) {
        RectangleShape tossRect(Vector2f(240.f, 199.f));
        tossRect.setPosition(556.f, 205.f);
        tossRect.setFillColor(Color::Black);
        tossRect.setOutlineColor(Color::White);
        tossRect.setOutlineThickness(2.f);
        window.draw(tossRect);

        print << " Click Here\nto Perform\n     TOSS";
        text.setPosition(560.f, 200.f);
        text.setString(print.str());;
        window.draw(text);
    }
    else {
        print << "  KILLS" << endl
            << "Player 1 = " << kill1 << endl
            << "Player 2 = " << kill2;
        text.setPosition(560.f, 200.f);
        text.setString(print.str());;
        window.draw(text);
        if (won == true && reading == false) {
            print2 << "Player " << result << " WON\n" <<
                "Press Escape to return to MENU";
            if (result == 2) {
                text.setFillColor(sf::Color(0, 255, 0));
            }
            else {
                text.setFillColor(sf::Color(255, 0, 0));
            }
            text.setPosition(110.f, 550.f);
            text.setString(print2.str());;
            window.draw(text);
        }
        else {
            print2 << "Player " << result << "'s TURN";
            text.setPosition(110.f, 550.f);
            text.setString(print2.str());
            if (result == 2) {
                text.setFillColor(sf::Color(0, 255, 0));
            }
            else {
                text.setFillColor(sf::Color(255, 0, 0));
            }
            window.draw(text);
        }
    }

    text.setFillColor(sf::Color(255, 255, 255));
}

void displayreading(RenderWindow& window) {
    countline();
    stringstream print3, print4;
    if (linescount != 0) {
        print3 << "Which game you want to play\nEnter number from 1 to " << linescount << ": ";
        if (key > 0) {
            print3 << key;
        }
        if (invalid == true) {
            print4 << "\nInvalid Input, Please Enter Again!!!!";
            text.setFillColor(sf::Color(244, 0, 0));
            text.setCharacterSize(47);
            text.setPosition(25.f, 579.f);
            text.setString(print4.str());;
            window.draw(text);

        }
    }
    else
        print3 << "No Save Game files available";
    text.setFillColor(sf::Color(255, 255, 255));
    text.setCharacterSize(42);
    text.setPosition(25.f, 540.f);
    text.setString(print3.str());;
    window.draw(text);
    text.setCharacterSize(55);
}


const int maxShapes = 25;
struct ShapeInfo {
    CircleShape shape;
    Vector2f velocity;
};

ShapeInfo shapes[maxShapes];

void initializeShapes() {
    for (int i = 0; i < maxShapes; ++i) {
        shapes[i].shape.setRadius(rand() % 30 + 10);
        shapes[i].shape.setFillColor(Color(rand() % 255, rand() % 255, rand() % 255));
        shapes[i].shape.setPosition(rand() % 800, rand() % 600);
        shapes[i].velocity = Vector2f(rand() % 2 + 1, rand() % 2 + 1);
    }
}

void updateShapes() {
    for (int i = 0; i < maxShapes; ++i) {
        shapes[i].shape.move(shapes[i].velocity);

        // Bounce off window boundaries
        if (shapes[i].shape.getPosition().x < 0 || shapes[i].shape.getPosition().x > 900 - 2 * shapes[i].shape.getRadius()) {
            shapes[i].velocity.x = -shapes[i].velocity.x;
        }
        if (shapes[i].shape.getPosition().y < 0 || shapes[i].shape.getPosition().y > 725 - 2 * shapes[i].shape.getRadius()) {
            shapes[i].velocity.y = -shapes[i].velocity.y;
        }
    }
}

//*main code
int main() {
    sf::RenderWindow window(sf::VideoMode(900, 725), "12 Bead Board", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    font.loadFromFile("Fonts/Dosis-Light.ttf");
    text.setCharacterSize(55);
    text.setFillColor(Color::White);
    text.setFont(font);
    result = toss() + 1;
    selectedBeadCoords.x = -1;
    selectedBeadCoords.y = -1;
    intialise();




    //Variables to show show movements
    Vector2f positition(0, 0), change(0, 0);
    bool moving = false;
    int tempturn;
    int finalX;
    int finalY;
    CircleShape movingcircle;
    int count;


    initializeShapes();

    while (window.isOpen()) {
        sf::Event event;
        //Adding background image
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("Picture/2.jpg")) {
            std::cerr << "Error loading background texture." << std::endl;
            return 1; // Return an error code
        }
        sf::Sprite backgroundSprite;
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setPosition(0.f, 0.f);
        backgroundSprite.setScale((920.f / backgroundTexture.getSize().x), (735.f / backgroundTexture.getSize().y));



        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed && reading == false) {
                menu = true;
            }
            else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape && reading == false) {
                    menu = true;
                }
            }
            if (event.type == sf::Event::TextEntered && reading == true && linescount != 0) {
                if (event.text.unicode != 13) {
                    if (event.text.unicode <= 57 && event.text.unicode >= 48) {
                        key *= 10;
                        key += event.text.unicode - 48;
                        cout << event.text.unicode - 48;
                    }
                    else if (event.text.unicode == 8) {
                        key /= 10;
                        cout << "\n" << key;

                    }
                }
                else {
                    start = false;

                    if (key > linescount || key == 0) {
                        key = 0;
                        invalid = true;
                    }

                    else {
                        invalid = false;
                        cout << "\nOpening file " << key;
                        linenum = key;

                        fileread();
                        menu = false;
                        showcont = true;
                        won = false;
                        reading = false;
                        if (checkWin(board, 1)) {
                            cout << "Player 2 wins!" << endl;
                            start = true;
                            showcont = false;
                            won = true;
                        }
                        else if (checkWin(board, 2)) {
                            cout << "Player 1 wins!" << endl;
                            start = true;
                            showcont = false;
                            won = true;
                        }
                        key = 0;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                BeadCoords.x = mouseX / (radius * 4);
                BeadCoords.y = mouseY / (radius * 4);
                if (mouseX - (120 * (BeadCoords.x)) <= 60 && mouseY - (120 * (BeadCoords.y)) <= 60 && BeadCoords.x >= 0 && BeadCoords.x <= 4 && BeadCoords.y >= 0 && BeadCoords.y <= 4 && board[BeadCoords.y][BeadCoords.x] == result && menu == false && reading == false && tossdone != false && won == false) {
                    cout << "Initial coordinates: ";
                    selectedBeadCoords.x = BeadCoords.x;
                    selectedBeadCoords.y = BeadCoords.y;
                    std::cout << selectedBeadCoords.y << "," << selectedBeadCoords.x << "\n";
                    isValid = valid(board, selectedBeadCoords.x, selectedBeadCoords.y, result);
                }
                if (mouseX - (120 * (BeadCoords.x)) <= 60 && mouseY - (120 * (BeadCoords.y)) <= 60 && BeadCoords.x >= 0 && BeadCoords.x <= 4 && BeadCoords.y >= 0 && BeadCoords.y <= 4 && board[BeadCoords.y][BeadCoords.x] == 0 && selectedBeadCoords.x != -1 && selectedBeadCoords.y != -1 && menu == false && reading == false && tossdone != false && won == false) {

                    finalX = BeadCoords.x;
                    finalY = BeadCoords.y;
                    cout << "\nFINAL COORDINATES\n";
                    cout << finalY << ", " << finalX;
                    isValid = check(board, finalY, finalX, selectedBeadCoords.y, selectedBeadCoords.x, result);
                    if (isValid == false) {
                        if (result == 1) {
                            board[selectedBeadCoords.y][selectedBeadCoords.x] = 0;
                            if (selectedBeadCoords.y - finalY == 2 || selectedBeadCoords.x - finalX == 2 || selectedBeadCoords.y - finalY == -2 || selectedBeadCoords.x - finalX == -2)
                                kill1 += 1;
                        }
                        else {
                            board[selectedBeadCoords.y][selectedBeadCoords.x] = 0;
                            if (selectedBeadCoords.y - finalY == 2 || selectedBeadCoords.x - finalX == 2 || selectedBeadCoords.y - finalY == -2 || selectedBeadCoords.x - finalX == -2)
                                kill2 += 1;
                        }
                        positition.x = selectedBeadCoords.x * (radius * 4);
                        positition.y = selectedBeadCoords.y * (radius * 4);
                        change.x = ((finalX - selectedBeadCoords.x) * (radius * 4)) / 12;
                        change.y = ((finalY - selectedBeadCoords.y) * (radius * 4)) / 12;
                        moving = true;
                        count = 0;

                        selectedBeadCoords.x = -1;
                        selectedBeadCoords.y = -1;
                    }
                }
                if (mouseX >= 188 && mouseX <= 541 && mouseY >= 35 && mouseY <= 84 && menu == true && showcont == true && reading == false) {
                    menu = false;
                    showcont = true;
                }
                if (mouseX >= 188 && mouseX <= 566 && mouseY >= 175 && mouseY <= 225 && menu == true) {
                    reading = false;
                    if (start == false)
                        filewrite();
                    intialise();
                    menu = false;
                    showcont = true;
                    won = false;
                    start = false;
                    tossdone = false;
                }
                if (mouseX >= 188 && mouseX <= 576 && mouseY >= 315 && mouseY <= 365 && menu == true && reading == false) {
                    if (start == false)
                        filewrite();
                    reading = true;
                    showcont = false;
                }
                if (mouseX >= 188 && mouseX <= 418 && mouseY >= 455 && mouseY <= 505 && menu == true) {
                    if (start == false && reading == false)
                        filewrite();
                    window.close();
                }
                if (mouseX >= 556 && mouseX <= 796 && mouseY >= 205 && mouseY <= 404 && tossdone == false) {
                    tossdone = true;
                }
            }
        }
        if (moving == true) {
            count += 1;
            movingcircle.setPosition(positition);
            positition.x += change.x;
            positition.y += change.y;

            movingcircle.setRadius(radius);
            if (result == 1)
                movingcircle.setFillColor(Color::Red);
            else
                movingcircle.setFillColor(Color(0, 220, 0));
            if (count == 12) {
                moving = false;
                board[finalY][finalX] = result;
                if (checkWin(board, 1)) {
                    cout << "Player 2 wins!" << endl;
                    result = (result == 1) ? 2 : 1;
                    showcont = false;
                    won = true;
                }
                else if (checkWin(board, 2)) {
                    cout << "Player 1 wins!" << endl;
                    result = (result == 1) ? 2 : 1;
                    showcont = false;
                    won = true;

                }
                result = (result == 1) ? 2 : 1;
                cout << "\nPLAYER " << result << "\n";
            }
        }



        updateShapes();
        window.clear(sf::Color(30, 30, 44));//background color add ho jata ha

        window.draw(backgroundSprite);
        if (menu == true) {
            for (int i = 0; i < maxShapes; ++i) {
                window.draw(shapes[i].shape);
            }
        }
        if (menu == false) {
            drawLines(window);
            drawBoard(window);
            if (selectedBeadCoords.y != -1 && selectedBeadCoords.x != -1)
                possibilities(board, selectedBeadCoords.y, selectedBeadCoords.x, result, window);
        }
        if (moving == true)
            window.draw(movingcircle);
        if (menu == false)
            display(window);
        else
            prntmenu(window);
        if (reading == true)
            displayreading(window);

        window.display();
    }
    return 0;
}