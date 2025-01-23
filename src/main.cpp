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


    // Make a bee sprite
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    textureBee.setSmooth(true);
    Sprite spriteBee(textureBee);
    spriteBee.setPosition({0,800});

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

        // Draw the spriteBackground
        window.draw(spriteBackground);

        // Draw the spriteTree
        window.draw(spriteTree);

        // Draw the spriteClouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw the spriteBee
        window.draw(spriteBee);

        window.display();
    }

    return 0;
}
