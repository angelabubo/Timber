
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

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
        if (spriteCloud.getPosition().x > 1920)
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

int main()
{
    //Create a videomode object
    VideoMode vm(1920, 1080);

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

    //Track whether the game is running
    bool paused = true;

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
        }

        //*******************************
        //Update the scene
        //*******************************
        if (!paused)
        {
            //Measure time; restart function also returns time elapsed when restarted
            Time dt = clock.restart();

            //Manage Bee
            ManageBee(beeActive, beeSpeed, dt, spriteBee);

            //Manage the clouds
            ManageCloud(cloud1Active, cloud1Speed, dt, spriteCloud1, 10);
            ManageCloud(cloud2Active, cloud2Speed, dt, spriteCloud2, 20);
            ManageCloud(cloud3Active, cloud3Speed, dt, spriteCloud3, 30);
        }
 
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

        //Draw the tree
        window.draw(spriteTree);

        //Draw the bee
        window.draw(spriteBee);

        //Show everything we just drew
        window.display();
    }
    return 0;
}

