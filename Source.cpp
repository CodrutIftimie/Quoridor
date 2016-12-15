#include <SFML/Graphics.hpp>
using namespace sf;
#define JUMP 75

void changePos(Sprite& player, Vector2f pos, int direction)
{
	if (direction == 0)
	{
		if (pos.x - JUMP >= 55 && pos.x - JUMP <= 670)
			player.setPosition(pos.x - JUMP, pos.y);
	}
	else if (direction == 1)
	{
		if (pos.x + JUMP >= 60 && pos.x + JUMP <= 670)
			player.setPosition(pos.x + JUMP, pos.y);
	}
	else if (direction == 2)
	{
		if (pos.y - JUMP >= 60 && pos.y - JUMP <= 670)
			player.setPosition(pos.x, pos.y - JUMP);
	}
	else if (direction == 3)
	{
		if (pos.y + JUMP >= 60 && pos.y + JUMP <= 670)
			player.setPosition(pos.x, pos.y + JUMP);
	}
}
int main()
{
	RenderWindow window(VideoMode(1024, 800), "Quoridor Game", Style::Close | Style::Titlebar);
	Texture MainBoard;
	Texture Player1Texture;
	Texture Player2Texture;
	Texture WallOrangeTexture;
	Texture WallBlueTexture;
	Vector2f pos;
	MainBoard.loadFromFile("Resources/Main_Table.jpg");
	Player1Texture.loadFromFile("Resources/Player1.png");
	Player2Texture.loadFromFile("Resources/Player2.png");
	WallOrangeTexture.loadFromFile("Resources/Wall-Orange-Horizontal.png");
	WallBlueTexture.loadFromFile("Resources/Wall-Blue-Horizontal.png");
	Sprite Board(MainBoard);
	Sprite Player1(Player1Texture);
	Sprite Player2(Player2Texture);
	Sprite WallOrange(WallOrangeTexture);
	Sprite WallBlue(WallBlueTexture);
	Player1.setPosition(364, 664);
	Player2.setPosition(359, 61);
	WallOrange.setPosition(799, 169);
	WallBlue.setPosition(799, 619);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
			{
				window.close();
				break;
			}
			case Event::KeyPressed:
			{
				pos = Player1.getPosition();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					changePos(Player1, pos, 0);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					changePos(Player1, pos, 1);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					changePos(Player1, pos, 2);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					changePos(Player1, pos, 3);
				break;
			}
			}
		}
		window.clear(Color::White);
		window.draw(Board);
		window.draw(Player1);
		window.draw(Player2);
		window.draw(WallOrange);
		window.draw(WallBlue);
		window.display();
	}
	return 0;
}