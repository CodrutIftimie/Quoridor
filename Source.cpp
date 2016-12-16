#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
#define JUMP 75

void changePos(Sprite& player, Vector2f playerPosition, int direction) // 0 - LEFT | 1 - RIGHT | 2 - UP | 3 - DOWN
{
	if (direction == 0)
	{
		if (playerPosition.x - JUMP >= 55 && playerPosition.x - JUMP <= 670)
			player.setPosition(playerPosition.x - JUMP, playerPosition.y);
	}
	else if (direction == 1)
	{
		if (playerPosition.x + JUMP >= 60 && playerPosition.x + JUMP <= 670)
			player.setPosition(playerPosition.x + JUMP, playerPosition.y);
	}
	else if (direction == 2)
	{
		if (playerPosition.y - JUMP >= 60 && playerPosition.y - JUMP <= 670)
			player.setPosition(playerPosition.x, playerPosition.y - JUMP);
	}
	else if (direction == 3)
	{
		if (playerPosition.y + JUMP >= 60 && playerPosition.y + JUMP <= 670)
			player.setPosition(playerPosition.x, playerPosition.y + JUMP);
	}
}

bool isSpriteClicked(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if(mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width && mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			printf("Wall clicked!\n");
			return true;
		}
		return false;
	}
	return false;
}

bool isBoardClicked(RenderWindow &Window, Event event)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && mousePos.x >= 64 && mousePos.x <= 723 && mousePos.y >= 68 && mousePos.y <= 731)
	{
		printf("Board Clicked\n");
		return true;
	}
	return false;
}

bool isInsideWall(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width && mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
		return true;
	return false;
}
int main()
{
	RenderWindow window(VideoMode(1024, 800), "Quoridor Game", Style::Close | Style::Titlebar);
	window.setFramerateLimit(30);
	Texture MainBoard;
	Texture PlayerTexture[2];
	Texture WallOrangeTexture;
	Texture WallBlueTexture;


	unsigned int index;
	unsigned short wallIndex[2] = { 0 };
	short board[9][9] = { 0 };
	bool playerPosMtx[9][9] = { 0 };
	playerPosMtx[0][4] = playerPosMtx[8][4] = 1;

	Vector2f playerPosition;
	Vector2f playerNewPosition;
	Vector2f wallPosition;


	MainBoard.loadFromFile("Resources/Main_Table.jpg");
	PlayerTexture[0].loadFromFile("Resources/Player1.png");
	PlayerTexture[1].loadFromFile("Resources/Player2.png");
	WallOrangeTexture.loadFromFile("Resources/Wall-Orange-Horizontal.png");
	WallBlueTexture.loadFromFile("Resources/Wall-Blue-Horizontal.png");

	Sprite Board(MainBoard);
	Sprite Player[2];

	Player[0].setTexture(PlayerTexture[0]);
	Player[1].setTexture(PlayerTexture[1]);
	Sprite Wall[2][10];
	for (index = 0; index < 10; index++)
	{
		Wall[0][index].setTexture(WallBlueTexture);
		Wall[1][index].setTexture(WallOrangeTexture);
	}

	Player[0].setPosition(364, 664);
	Player[1].setPosition(359, 61);
	Wall[1][0].setPosition(799, 169);
	Wall[0][0].setPosition(799, 619);
	for (index = 1; index < 10; index++)
	{
		Wall[1][index].setPosition(1025, 169);
		Wall[0][index].setPosition(1025, 619);
	}

	bool playerTurn = false;
	bool wallBeingPlaced = false;
	bool click = false;
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
					playerPosition = Player[playerTurn].getPosition();
					if (((Keyboard::isKeyPressed(Keyboard::Left) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::A) && playerTurn == 1)) && wallBeingPlaced == false)
						changePos(Player[playerTurn], playerPosition, 0);
					else if (((Keyboard::isKeyPressed(Keyboard::Right) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::D) && playerTurn == 1)) && wallBeingPlaced == false)
						changePos(Player[playerTurn], playerPosition, 1);
					else if (((Keyboard::isKeyPressed(Keyboard::Up) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::W) && playerTurn == 1)) && wallBeingPlaced == false)
						changePos(Player[playerTurn], playerPosition, 2);
					else if (((Keyboard::isKeyPressed(Keyboard::Down) && playerTurn == 0) || (Keyboard::isKeyPressed(Keyboard::S) && playerTurn == 1)) && wallBeingPlaced == false)
						changePos(Player[playerTurn], playerPosition, 3);
					playerNewPosition = Player[playerTurn].getPosition();
					if(playerNewPosition != playerPosition)
						playerTurn = !playerTurn;
					break;
				}
				case Event::MouseButtonReleased:
				{
					if (isInsideWall(Wall[playerTurn][wallIndex[playerTurn]],window) == false && wallBeingPlaced == true && isBoardClicked(window, event))
					{
						if (event.mouseButton.button == sf::Mouse::Left)
						{
							Vector2i mousePos = Mouse::getPosition(window);
							Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x, mousePos.y);
							wallIndex[playerTurn]++;
							playerTurn = !playerTurn;
							wallBeingPlaced = false;
						}
					}
					break;
				}
				default: {
					if (wallBeingPlaced == true)
					{
						Vector2i mousePos = Mouse::getPosition(window);
						Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x, mousePos.y);
					}
					else if (wallBeingPlaced == false && isSpriteClicked(Wall[playerTurn][wallIndex[playerTurn]], window))
					{
						wallBeingPlaced = true;
						if(playerTurn == false)
							Wall[0][wallIndex[0] + 1].setPosition(799, 619);
						else Wall[1][wallIndex[1] + 1].setPosition(799, 169);
					}
					break; 
				}
			}
		}
		window.clear(Color::White);
		window.draw(Board);
		window.draw(Player[1]);
		for (index = 9; index > 0; index--)
		{
			window.draw(Wall[1][index]);
			window.draw(Wall[0][index]);
		}
		window.draw(Player[0]);
		window.draw(Wall[1][0]);
		window.draw(Wall[0][0]);
		window.display();
	}
	return 0;
}