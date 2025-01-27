#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

using namespace sf;


// Functions
void centerMessageText(Text& messageText, float windowWidth, float windowHeight) {
    // Reposition the text based on its new size
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin({
        textRect.position.x + textRect.size.x / 2.0f, // x position
        textRect.position.y + textRect.size.y / 2.0f  // y position
    });
    messageText.setPosition({1920 / 2.0f, 1080 / 2.0f}); // Center on screen
}


void newGame(int& score, float& timeRemaining, bool& paused, bool& gameOver, bool& beeActive, bool& cloud1Active, bool& cloud2Active, bool& cloud3Active, Text& messageText, RectangleShape& timeBar, const float timeBarStartWidth, const float timeBarHeight) {
    // Reset game variables
    score = 0;
    timeRemaining = 5.0f;
    beeActive = false;
    cloud1Active = false;
    cloud2Active = false;
    cloud3Active = false;

    // Reset the message text
    messageText.setString("Press Enter to Start");
    centerMessageText(messageText, 1920, 1080);

    // Reset the time bar
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));

    // Unpause and reset the game state
    paused = false;
    gameOver = false;
}


void togglePauseGame(bool& paused, Clock& clock) {
    paused = !paused;
    clock.restart();
}


void stopGame(bool& paused, Text& messageText) {
    // Pause the game
    paused = true;

    // Change the message to show "Game Over" or any other text
    messageText.setString("Game Over! Press R to Restart or Escape to Exit");

    centerMessageText(messageText, 1920, 1080);
}


void exitGame(RenderWindow& window) {
    // Close the game window
    window.close();
}

int main()
{
    RenderWindow window(VideoMode({1920, 1080}), "SFML 3 - Timber");

    // Load a texture
    Texture textureBackground;
    if (!textureBackground.loadFromFile("graphics/background.png"))
    {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }

    // Set textureBackground smooth
    textureBackground.setSmooth(true);

    // Create a sprite and attach the texture
    Sprite spriteBackground(textureBackground);

    // Get the size of the texture and the window
    Vector2u textureBackgroundSize = textureBackground.getSize();
    Vector2u windowSize = window.getSize();

    // Calculate scale factors
    float scaleX = static_cast<float>(windowSize.x) / textureBackgroundSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureBackgroundSize.y;

    // Debugging the scale factors
    std::cout << "Scale factors - X: " << scaleX << ", Y: " << scaleY << std::endl;

    // Apply the scaling to fit the window
    spriteBackground.setScale({scaleX, scaleY});


    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    textureTree.setSmooth(true);
    Sprite spriteTree(textureTree);
    spriteTree.setPosition({810, 0});

    
    // Make clouds sprite
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    textureCloud.setSmooth(true);
    // 3 sprites with the same texture for clouds
    Sprite spriteCloud1(textureCloud);
    Sprite spriteCloud2(textureCloud);
    Sprite spriteCloud3(textureCloud);
    spriteCloud1.setPosition({0,0});
    spriteCloud2.setPosition({0,250});
    spriteCloud3.setPosition({0,500});
    // Are the clouds currently on screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    // How fast is each cloud?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;


    // Make a bee sprite
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    textureBee.setSmooth(true);
    Sprite spriteBee(textureBee);
    spriteBee.setPosition({0,800});
    // Is the bee currently moving?
    bool beeActive = false;
    // How fast can the bee fly?
    float beeSpeed = 0.0f;


    // Choose a font
    Font font;
    font.openFromFile("fonts/KOMIKAP_.ttf");


    // Set up the message text
    Text messageText(font);
    messageText.setString("Press Enter to Start");
    messageText.setCharacterSize(75); // Character size in pixels
    messageText.setFillColor(Color::White);
    messageText.setStyle(Text::Bold);
    messageText.setPosition({1920 / 2 - 350, 1080 / 2 - 50}); // Centered position


    // Set up the score text
    int score = 0;
    Text scoreText(font);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(100);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition({20, 20}); // Top-left corner


    // Variables to control time itself
    Clock clock;

    // Timebar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition({(1920/2) - timeBarStartWidth / 2, 980});

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


    // Track whether the game is running
    bool paused = true;
    bool gameOver = true;


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // Toggle pause with Escape key
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    togglePauseGame(paused, clock);
                }


                if (keyPressed->scancode == sf::Keyboard::Scancode::Enter && gameOver) {
                    newGame(score, timeRemaining, paused, gameOver, beeActive, cloud1Active, cloud2Active, cloud3Active, messageText, timeBar, timeBarStartWidth, timeBarHeight);
                }
            }
        }


        // Exit the game
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            exitGame(window);
        }


        if (!paused && !gameOver) {
            // Measure time
            Time dt = clock.restart();

            // Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // Size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                stopGame(paused, messageText);

                if (Keyboard::isKeyPressed(Keyboard::Key::R)) {
                    newGame(score, timeRemaining, paused, gameOver, beeActive, cloud1Active, cloud2Active, cloud3Active, messageText, timeBar, timeBarStartWidth, timeBarHeight);
                }
            }


            // Setup the cloud1
            if (!cloud1Active) {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200) + 25;

                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 450) - 150;
                spriteCloud1.setPosition({-200, height});
                cloud1Active = true;

            } else {
                spriteCloud1.setPosition({spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y});

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud1.getPosition().x > 1920) {
                    cloud1Active = false;
                }
            }

            // Setup the cloud2
            if(!cloud2Active) {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200) + 25;

                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition({-200, height});
                cloud2Active = true;

            } else {
                spriteCloud2.setPosition({spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y});

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud2.getPosition().x > 1920) {
                    cloud2Active = false;
                }
            }

            // Setup the cloud3
            if(!cloud3Active) {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200) + 25;

                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition({-200, height});
                cloud3Active = true;

            } else {
                spriteCloud3.setPosition({spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y});

                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud3.getPosition().x > 1920) {
                    cloud3Active = false;
                }
            }


            // Setup the bee
            if (!beeActive) {
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200;


                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition({2000, height});
                beeActive = true;

            } else {
                spriteBee.setPosition({spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y});

                // Has the bee reached the left hand edge of the screen?
                if (spriteBee.getPosition().x < -100) {
                    beeActive = false;
                }
            }


            // Update the score text
            std::stringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());


        }

        // Draw the spriteBackground
        window.draw(spriteBackground);
        
        // Draw the spriteClouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw the spriteTree
        window.draw(spriteTree);

        // Draw the spriteBee
        window.draw(spriteBee);
        
        // Draw the score
        window.draw(scoreText);

        // Draw the timebar
        window.draw(timeBar);

        if (paused) {
            // Draw our message
            window.draw(messageText);
        }

        window.display();
    }

    return 0;
}
