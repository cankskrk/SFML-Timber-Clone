#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

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


    // Variables to control time itself
    Clock clock;


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();

        // Measure time
        Time dt = clock.restart();


        // Setup the cloud1
        if(!cloud1Active) {
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

        window.display();
    }

    return 0;
}
