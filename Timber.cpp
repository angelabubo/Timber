
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;
using namespace std;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//Where is the player or branch?
//Left or right?
enum class side {
    LEFT,
    RIGHT,
    NONE
};

side branchPositions[NUM_BRANCHES];

void ManageCloud(bool& cloudActive, float& cloudSpeed, Time& dt, Sprite& spriteCloud, const int& seed)
{
    if (!cloudActive)
    {
        //How fast is the cloud?
        srand((int)time(0) * seed);
        cloudSpeed = (rand() % 200);

        //How high is the cloud?
        srand((int)time(0) * seed);
        float height = (rand() % 150);
        spriteCloud.setPosition(-200, height);
        cloudActive = true;
    }
    else
    {
        spriteCloud.setPosition(
            spriteCloud.getPosition().x +
            (cloudSpeed * dt.asSeconds()),
            spriteCloud.getPosition().y
        );

        //Has the cloud reached the right hand edge of the screen?
        if (spriteCloud.getPosition().x > WINDOW_WIDTH)
        {
            cloudActive = false;
        }
    }
}
void ManageBee(bool& beeActive, float& beeSpeed, Time& dt, Sprite& spriteBee) {
    if (!beeActive)
    {
        //How fast is the bee?
        srand((int)time(0) * 10);
        beeSpeed = (rand() % 200) + 200;

        //How high is the bee?
        srand((int)time(0) * 10);
        float height = (rand() % 500) + 500;
        spriteBee.setPosition(2000, height);
        beeActive = true;
    }
    else
    {
        spriteBee.setPosition(
            spriteBee.getPosition().x -
            (beeSpeed * dt.asSeconds()),
            spriteBee.getPosition().y
        );

        //Has the bee reached the right hand edge of the screen?
        if (spriteBee.getPosition().x < -100)
        {
            beeActive = false;
        }

    }
}
void PositionMessage(Text& messageText) {
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    messageText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
}
void UpdateBranches(int seed)
{
    //Move the branches down one place
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
    {
        branchPositions[i] = branchPositions[i - 1];
    }

    //Spawn a new branch at position 0
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}



int main()
{
    //Create a videomode object
    VideoMode vm(WINDOW_WIDTH, WINDOW_HEIGHT);

    //Create and open a window for the game
    RenderWindow window(vm, "Timber!!", Style::Fullscreen);

    //Create a texture to hold a graphic on the GPU
    Texture textureBackground;

    //Load a graphic into the texture
    textureBackground.loadFromFile("graphics/background.png");

    //Create a sprite
    Sprite spriteBackground;

    //Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    //Set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);

    //Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");

    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    //Is the bee currently moving?
    bool beeActive = false;

    //How fast can the bee fly?
    float beeSpeed = 0.0f;

    //Make 3 clouds from 1 texture
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    //3 cloud sprites from the same texture
    Sprite spriteCloud1, spriteCloud2, spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    //Position the sprite clouds in the screen
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    //Are the clouds currently onscreen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    //How fast is each cloud?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    //Variable to control time itself
    Clock clock;

    //Time bar
    RectangleShape timeBar;
    
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((WINDOW_WIDTH / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //Track whether the game is running
    bool paused = true;

    //Draw some text
    int score = 0;
    Text messageText;
    Text scoreText;

    //Choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    //Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);

    //Assign the actual message
    messageText.setString("Press Enter to start the game.");
    scoreText.setString("Score = 0");

    //Increase font size
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    //Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    //Position the text
    PositionMessage(messageText);
    scoreText.setPosition(20, 20);

    //Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    //Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);

        //Hide it offscreen
        branches[i].setPosition(-2000, -2000);

        //Set the sprite's origin to dead center
        //We can then spin it around without moving its position
        branches[i].setOrigin(220, 20);
    }

    //Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    //The player starts on the left
    side playerSide = side::LEFT;

    //Prepare the gravestone
    Texture textureGameOver;
    textureGameOver.loadFromFile("graphics/rip.png");
    Sprite spriteGameOver;
    spriteGameOver.setTexture(textureGameOver);
    spriteGameOver.setPosition(600, 860);

    //Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    //Line the axe up with the tree

    while (window.isOpen())
    {
        //*******************************
        //Handle player's input
        //*******************************

        //Exit the game
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        //Start the game
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            paused = false;

            //Reset the time and score
            score = 0;
            timeRemaining = 5;
        }

        //*******************************
        //Update the scene
        //*******************************
        if (!paused)
        {
            //Measure time; restart function also returns time elapsed when restarted
            Time dt = clock.restart();

            //Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();

            //sizeup the timebar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                //Pause the game
                paused = true;

                //Change message shown to player
                messageText.setString("Out of time!");

                //Reposition the text based on its new size
                PositionMessage(messageText);
            }


            //Manage Bee
            ManageBee(beeActive, beeSpeed, dt, spriteBee);

            //Manage the clouds
            ManageCloud(cloud1Active, cloud1Speed, dt, spriteCloud1, 10);
            ManageCloud(cloud2Active, cloud2Speed, dt, spriteCloud2, 20);
            ManageCloud(cloud3Active, cloud3Speed, dt, spriteCloud3, 30);

            //Update the score text
            stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            //Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    //Move the sprite to the left side
                    branches[i].setPosition(610, height);
                    //Flip the sprite round the other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    //Move the sprite to the right side
                    branches[i].setPosition(1330, height);
                    //Set sprite rotation to normal
                    branches[i].setRotation(0);
                } 
                else
                {
                    //Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }

        } //end if paused
 
        //*******************************
        //Draw the scene
        //*******************************

        //Clear everything from the last frame
        window.clear();

        //Draw our game scene here
        window.draw(spriteBackground);

        //Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        //Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        //Draw the tree
        window.draw(spriteTree);

        //Draw the bee
        window.draw(spriteBee);

        //Draw the score
        window.draw(scoreText);

        //Draw the timebar
        window.draw(timeBar);

        //Draw the message if paused
        if (paused)
        {
            window.draw(messageText);
        }

        //Show everything we just drew
        window.display();
    }
    return 0;
}

