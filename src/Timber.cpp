#include "sstream"
#include "array"
#include "iostream"

// include SFML libraries
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

using namespace sf;


// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;

// Where is the player/branch?
// Left or right
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

// ****** This is where our game starts from *****

int main()
{

    // create a video mode object
    VideoMode vm({1920, 1080});

    // create and open a window for the game
    RenderWindow window(vm, "SFML 3 - Timber");

    // Create a texture to hold a graphic on the GPU
    Texture textureBackground;

    if (!textureBackground.loadFromFile("graphics/background.png")) {
        // Handle the error, e.g., log it or exit the program
        std::cerr << "Error loading background texture" << std::endl;
        return -1;
    }

    // Create a sprite
    Sprite spriteBackground(textureBackground);

    // Set the spriteBackground to cover the screen
    spriteBackground.setPosition({0, 0});


    // Make a tree sprite
    Texture textureTree;
    if (!textureTree.loadFromFile("graphics/tree.png")) {
        // Handle the error
        std::cerr << "Error loading tree texture" << std::endl;
        return -1;
    }

    Sprite spriteTree(textureTree);
    spriteTree.setPosition({810, 0});

    // Prepare the bee
    Texture textureBee;
    if (!textureBee.loadFromFile("graphics/bee.png")) {
        // Handle the error
        std::cerr << "Error loading bee texture" << std::endl;
        return -1;
    }

    Sprite spriteBee(textureBee);
    spriteBee.setPosition({0, 800});

    // is the bee currently moving?
    bool beeActive = false;

    // How fast can the bee fly?
    float beeSpeed = 0.0f;



    // *** make 3 cloud sprites from 1 texture

    // create a new texture object
    Texture textureCloud;

    // load 1 texture
    textureCloud.loadFromFile("graphics/cloud.png");

    // 3 new sprites with the same texture
    Sprite spriteCloud1(textureCloud);
    Sprite spriteCloud2(textureCloud);
    Sprite spriteCloud3(textureCloud);

    // position the clouds on the left of the screen at different heights
    spriteCloud1.setPosition({0, 0});
    spriteCloud2.setPosition({0, 250});
    spriteCloud3.setPosition({0, 500});

    // Are the clouds currently on screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    // how fast is each cloud?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // variable to control time itself
    Clock clock;


    // Time bar

    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition({(1920 / 2) - timeBarStartWidth / 2, 980});

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;



    // track whether the game is running
    bool paused = true;

    // draw some text
    int score = 0;

    // we need to choose a font
    Font font;
    if (!font.openFromFile("fonts/KOMIKAP_.ttf")) {
        // Handle the error
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    Text messageText(font);
    Text scoreText(font);

    // assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    // make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);


    // position the text
    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});

    messageText.setPosition({1920 / 2.0f, 1080 / 2.0f});

    scoreText.setPosition({20, 20});


    // Load the branch texture
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    Sprite branches[NUM_BRANCHES] = {
        Sprite(textureBranch),
        Sprite(textureBranch),
        Sprite(textureBranch),
        Sprite(textureBranch),
        Sprite(textureBranch),
        Sprite(textureBranch)
    };

    // Initialize each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition({-2000, -2000}); // Off-screen initially
        branches[i].setOrigin({220, 20});
    }



    // prepare the player 
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer(texturePlayer);
    spritePlayer.setPosition({580, 720});

    // the player starts on the left
    side playerSide = side::LEFT;

    // prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP(textureRIP);
    spriteRIP.setPosition({600, 860});

    // prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe(textureAxe);
    spriteAxe.setPosition({700, 830});

    // line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog(textureLog);
    spriteLog.setPosition({810, 720});


    // some other useful log related variable
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;


    // control the player input 
    bool acceptInput = false;


    // prepare the sounds
    // the player chopping sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop(chopBuffer);

    // the player has met his end under a branch
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death(deathBuffer);

    // out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime(ootBuffer);


    while (window.isOpen())
    {
        /*
        *******************************************************
        *   Handle the players input
        *******************************************************
        */


        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::KeyReleased>() && !paused)
            {
                // Listen for key presses again
                acceptInput = true;

                // hide the axe
                spriteAxe.setPosition({2000, spriteAxe.getPosition().y});

            }
        }






        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }

        // START THE GAME
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
        {
            paused = false;

            // reset the time and the score
            score = 0;
            timeRemaining = 6;

            // make all the branches disappear - starting in the second position
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }


            // make sure the gravestone is hidden
            spriteRIP.setPosition({675, 2000});

            // move the player into position
            spritePlayer.setPosition({580, 720});

            acceptInput = true;

        }



        // wrap the player controls to make sure we are accepting input
        if (acceptInput)
        {
            // more code here next...

            // first handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                // make sure the player is on the right
                playerSide = side::RIGHT;
                score++;

                // add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition({AXE_POSITION_RIGHT, spriteAxe.getPosition().y});

                spritePlayer.setPosition({1200, 720});

                // update the branches
                updateBranches(score);

                // set the log flying to the left
                spriteLog.setPosition({810, 720});
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                // play a chop sound
                chop.play();

            }

            // handle the left cursor key

            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                // make sure the player is on the left
                playerSide = side::LEFT;

                score++;

                // add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition({AXE_POSITION_LEFT, spriteAxe.getPosition().y});

                spritePlayer.setPosition({580, 720});

                // update the branches
                updateBranches(score);

                // set the log flying
                spriteLog.setPosition({810, 720});
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                // play a chop sound
                chop.play();

            }
        }




        /*
        *******************************************************
        *   Update the scene
        *******************************************************
        */

        if (!paused)
        {

            // measure time
            Time dt = clock.restart();

            // substract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // resize up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                // Pause the game
                paused = true;

                // Change the message shown to the player
                messageText.setString("Out of time!!");

                //Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});

                messageText.setPosition({1920 / 2.0f, 1080 / 2.0f});

                // play the out of time sound
                outOfTime.play();

            }




            // setup the bee
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition({2000, height});
                beeActive = true;
            }
            else
                // Move the bee
            {
                spriteBee.setPosition({spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y});


                // has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    // set it up ready to be a whole new bee, next frame
                    beeActive = false;
                }
            }


            // *** manage the clouds ***
            // 
            // 
            // Cloud 1
            if (!cloud1Active)
            {
                // how fast is the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                // how high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition({-200, height});
                cloud1Active = true;

            }
            else
            {
                spriteCloud1.setPosition({
                    spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y});


                // has the cloud reached the right hand edge of the screen?
                if (spriteCloud1.getPosition().x > 1920)
                {
                    // set it up ready to be a whole new cloud next frame
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active)
            {
                // how fast is the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                // how high is the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition({-200, height});
                cloud2Active = true;

            }
            else
            {
                spriteCloud2.setPosition({
                    spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y});


                // has the cloud reached the right hand edge of the screen?
                if (spriteCloud2.getPosition().x > 1920)
                {
                    // set it up ready to be a whole new cloud next frame
                    cloud2Active = false;
                }
            }

            // Cloud 3
            if (!cloud3Active)
            {
                // how fast is the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                // how high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition({-200, height});
                cloud3Active = true;

            }
            else
            {
                spriteCloud3.setPosition({
                    spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y});


                // has the cloud reached the right hand edge of the screen?
                if (spriteCloud3.getPosition().x > 1920)
                {
                    // set it up ready to be a whole new cloud next frame
                    cloud3Active = false;
                }
            }



            // update the score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());



            // update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;

                if (branchPositions[i] == side::LEFT)
                {
                    // move the sprite to the left side
                    branches[i].setPosition({610, height});

                    // flip the sprite round the other way
                    branches[i].setRotation(degrees(180));
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    // move the sprite to the right side
                    branches[i].setPosition({1330, height});

                    // set the sprite rotation to normal
                    branches[i].setRotation(degrees(0));
                }
                else
                {
                    // hide the branch
                    branches[i].setPosition({3000, height});
                }
            }

            // handle a flying log
            if (logActive)
            {
                spriteLog.setPosition
                ({
                    spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
                });
            }

            // has the log reached the right hand edge?
            if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
            {
                // set it up ready to be a whole new log next frame
                logActive = false;
                spriteLog.setPosition({810, 720});
            }

            // has the player been squished by a branch?
            if (branchPositions[5] == playerSide)
            {
                // death



                paused = true;
                acceptInput = false;

                // draw the gravestone
                spriteRIP.setPosition({525, 760});

                // hide the player
                spritePlayer.setPosition({2000, 660});

                // change the text of the message
                messageText.setString("SQUISHED!");

                // center it on the screen
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});

                messageText.setPosition({1920 / 2.0f, 1080 / 2.0f});

                // play the death sound
                death.play();




            }



        } // end if(!paused)


       



        /*
        *******************************************************
        *   Draw the scene
        *******************************************************
        */

        // Clear everything from the last run frame 
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);

        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);


        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }


        // Draw the tree
        window.draw(spriteTree);


        // draw the player
        window.draw(spritePlayer);

        // draw the axe
        window.draw(spriteAxe);

        // draw the flying log
        window.draw(spriteLog);
        
        // draw the gravestone
        window.draw(spriteRIP);

        // draw the bee
        window.draw(spriteBee);






        // Draw the insect
        window.draw(spriteBee);

        // Draw the score
        window.draw(scoreText);

        // Draw the timebar
        window.draw(timeBar);


        if (paused)
        {
            // Draw our message
            window.draw(messageText);
        }


 

        // Show everything we just drew
        window.display();

    }

    return 0;
}

// function definition
void updateBranches(int seed)
{
    // move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // spawn a new branch at position 0
    // LEFT, RIGHT or NONE
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