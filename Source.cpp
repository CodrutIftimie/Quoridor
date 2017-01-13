#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <time.h>
using namespace sf;
#define JUMP 77
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define TWOPLAYERS false
#define FOURPLAYERS true

void changeGraphicalPos(Sprite& player, Vector2i playerPosMatrix);
bool changeMatrixPos(Vector2i &playerPosMatrix, Vector2i player2PosMatrix, bool boardMatrix[17][17], int direction);
int changeMatrixPos4Players(Vector2i playerPosMatrix[4], short playerTurn, bool boardMatrix[17][17], int direction);
void wallPlacementPrediction(Vector2i mousePos, Vector2i &prediction);
bool isSpriteClicked(Sprite Wall, RenderWindow &Window);
bool isBoardClicked(RenderWindow &Window, Event event);
bool isInsideWall(Sprite Wall, RenderWindow &Window);
bool isAbleToMove(bool boardMatrix[17][17], Vector2i playerPos, int direction);
bool isAllowedToPlaceWall(bool boardMatrix[17][17], Vector2i prediction, bool wallFacing);
bool pressedToMove(int direction, short playerTurn, bool gameMode, Event event);
void searchRoute(bool boardMatrix[17][17], int playerY, int playerX, short playerTurn, int &result, int visited[17][17]);
void createTemporaryWall(bool boardMatrix[17][17], Vector2i WallPrediction, bool wallFacing);
bool isInBorders(int playerY, int playerX);
void changePlayerTurn(short &playerTurn, bool gameMode);
void emptyMatrix(int matrix[17][17]);
int oppositePlayer(short playerTurn, bool gameMode);
bool modifyWaitForInput(bool gameMode, short playerTurn, Vector2i playerPosMatrix[4], bool boardMatrix[17][17], int direction);
bool freeSpace(Vector2i playerPosMatrix[4], Vector2i nextPos, int direction, bool gameMode);

int main()
{
	bool gameMode;
	bool gameStarted = false;
	bool firstTime = true;
	bool gameOver = false;
	bool gameOverFirstTime = false;
	RenderWindow gameWindow(VideoMode(950, 730), "Quoridor Game", Style::Close | Style::Titlebar);
	gameWindow.setFramerateLimit(45);
	bool boardMatrix[17][17] = { false };
	short playerTurn;
	short winner;
	bool wallBeingPlaced = false;
	bool WallFacing = true;
	bool change = false;
	bool waitForInput = false;
	Texture MainBoard;
	Texture PlayerTexture[4];
	Texture WallOrangeTextureH;
	Texture WallOrangeTextureV;
	Texture WallBlueTextureH;
	Texture WallBlueTextureV;
	Texture WallGreenTextureH;
	Texture WallGreenTextureV;
	Texture WallRedTextureH;
	Texture WallRedTextureV;
	Texture WallPlacementTextureH;
	Texture WallPlacementTextureV;

	Text wallsLeft[4];
	Font arial;
	String amount;

	arial.loadFromFile("Resources/arial.ttf");
	for (int i = 0; i < 4; i++)
	{
		wallsLeft[i].setFont(arial);
		wallsLeft[i].setFillColor(Color::Black);
		wallsLeft[i].setCharacterSize(15);
		wallsLeft[i].setPosition(925, 0);
	}

	MainBoard.loadFromFile("Resources/MainMenu.png");

	unsigned int index;

	unsigned short wallIndex[4] = { 0 };
	int direction;

	Vector2i playerPosMatrix[4];

	Sprite Board(MainBoard);
	Sprite Player[4];
	Sprite Wall[4][10];
	Sprite WallPlacement;
	Vector2i playerNewPosition;
	Vector2i WallPrediction;

	while (gameWindow.isOpen())
	{
		Event event;
		while (gameWindow.pollEvent(event))
		{
			if (gameStarted == true && firstTime == true)
			{
				srand(time(NULL));
				if (gameMode == TWOPLAYERS)
				{
					playerTurn = rand() % 2;
					if (playerTurn == 1)
						++playerTurn;
				}
				else playerTurn = rand() % 4;
				printf("%d", playerTurn);
				playerPosMatrix[0].x = 8;
				playerPosMatrix[0].y = 16;
				playerPosMatrix[2].x = 8;
				playerPosMatrix[2].y = 0;
				if (gameMode == FOURPLAYERS)
				{
					playerPosMatrix[1].x = 0;
					playerPosMatrix[1].y = 8;
					playerPosMatrix[3].x = 16;
					playerPosMatrix[3].y = 8;
				}
				if (gameMode == TWOPLAYERS)
					MainBoard.loadFromFile("Resources/MainBoard.png");
				else MainBoard.loadFromFile("Resources/MainBoard4Players.png");
				PlayerTexture[0].loadFromFile("Resources/Player-Orange.png");
				PlayerTexture[1].loadFromFile("Resources/Player-Green.png");
				PlayerTexture[2].loadFromFile("Resources/Player-Blue.png");
				PlayerTexture[3].loadFromFile("Resources/Player-Red.png");
				WallOrangeTextureH.loadFromFile("Resources/Wall-Orange-Horizontal.png");
				WallOrangeTextureV.loadFromFile("Resources/Wall-Orange-Vertical.png");
				WallBlueTextureH.loadFromFile("Resources/Wall-Blue-Horizontal.png");
				WallBlueTextureV.loadFromFile("Resources/Wall-Blue-Vertical.png");
				WallGreenTextureH.loadFromFile("Resources/Wall-Green-Horizontal.png");
				WallGreenTextureV.loadFromFile("Resources/Wall-Green-Vertical.png");
				WallRedTextureH.loadFromFile("Resources/Wall-Red-Horizontal.png");
				WallRedTextureV.loadFromFile("Resources/Wall-Red-Vertical.png");
				WallPlacementTextureH.loadFromFile("Resources/Wall-Position-Horizontal.png");
				WallPlacementTextureV.loadFromFile("Resources/Wall-Position-Vertical.png");

				WallPlacement.setPosition(1000, 1000);
				WallPlacement.setTexture(WallPlacementTextureH);

				Player[0].setTexture(PlayerTexture[0]);
				Player[2].setTexture(PlayerTexture[2]);
				if (gameMode == FOURPLAYERS)
				{
					Player[1].setTexture(PlayerTexture[1]);
					Player[3].setTexture(PlayerTexture[3]);
				}
				if (gameMode == TWOPLAYERS)
					index = 10;
				else index = 5;
				for (int i = 0; i < index; i++)
				{
					Wall[0][i].setTexture(WallOrangeTextureH);
					Wall[1][i].setTexture(WallGreenTextureH);
					Wall[2][i].setTexture(WallBlueTextureH);
					Wall[3][i].setTexture(WallRedTextureH);
				}

				changeGraphicalPos(Player[0], playerPosMatrix[0]);
				changeGraphicalPos(Player[2], playerPosMatrix[2]);
				if (gameMode == FOURPLAYERS)
				{
					changeGraphicalPos(Player[1], playerPosMatrix[1]);
					changeGraphicalPos(Player[3], playerPosMatrix[3]);
				}
				if (gameMode == TWOPLAYERS)
					amount = "10";
				else amount = "5";
				Wall[0][0].setPosition(762, 591);
				Wall[2][0].setPosition(762, 283);
				wallsLeft[0].setPosition(925, 588);
				wallsLeft[2].setPosition(925, 280);
				wallsLeft[0].setString(amount);
				wallsLeft[2].setString(amount);
				if (gameMode == FOURPLAYERS)
				{
					Wall[1][0].setPosition(762, 437);
					Wall[3][0].setPosition(762, 129);
					wallsLeft[1].setPosition(925, 434);
					wallsLeft[3].setPosition(925, 126);
					wallsLeft[0].setString(amount);
					wallsLeft[1].setString(amount);
					wallsLeft[2].setString(amount);
					wallsLeft[3].setString(amount);
				}
				if (gameMode == TWOPLAYERS)
					for (index = 1; index < 10; index++)
					{
						Wall[2][index].setPosition(1025, 283);
						Wall[0][index].setPosition(1025, 591);
					}
				else
					for (index = 1; index < 5; index++)
					{
						Wall[2][index].setPosition(1025, 283);
						Wall[0][index].setPosition(1025, 591);
						Wall[1][index].setPosition(1025, 437);
						Wall[3][index].setPosition(1025, 129);
					}
				firstTime = false;
			}
			else if (gameOver == true)
			{
				if (gameOverFirstTime == true)
				{
					MainBoard.loadFromFile("Resources/GameOver.png");
					for (index = 0; index < 4; index++)
					{
						Player[index].setPosition(1000, 1000);
						wallsLeft[index].setPosition(1000, 1000);
						for (int index2 = 0; index2 < 10; index2++)
							Wall[index][index2].setPosition(1000, 1000);
					}
					Player[winner].setPosition(335, 350);
					gameOverFirstTime = false;
				}
				Vector2i mousePos = Mouse::getPosition(gameWindow);
				if (mousePos.x >= 325 && mousePos.y >= 520 && mousePos.x <= 624 && mousePos.y <= 579)
				{
					if (event.mouseButton.button == Mouse::Left)
					{
						MainBoard.loadFromFile("Resources/MainMenu.png");
						Player[winner].setPosition(1000, 1000);
						gameStarted = false;
						firstTime = true;
						gameOver = false;
					}
				}
			}
			else
			{
				switch (event.type)
				{
				case Event::Closed:
				{
					gameWindow.close();
					break;
				}
				case Event::KeyPressed:
				{
					if (gameStarted == true)
					{
						if (waitForInput == true)
						{
							bool unmovable = false;
							if (gameMode == TWOPLAYERS)
							{
								if (pressedToMove(UP, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, playerPosMatrix[oppositePlayer(playerTurn, gameMode)], UP))
								{
									if (direction == LEFT)
									{
										playerPosMatrix[playerTurn].x -= 2;
										playerPosMatrix[playerTurn].y -= 2;
										waitForInput = false;
									}
									else if (direction == RIGHT)
									{
										playerPosMatrix[playerTurn].x += 2;
										playerPosMatrix[playerTurn].y -= 2;
										waitForInput = false;
									}
								}
								else if (pressedToMove(DOWN, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, playerPosMatrix[oppositePlayer(playerTurn, gameMode)], DOWN))
								{
									if (direction == LEFT)
									{
										playerPosMatrix[playerTurn].x -= 2;
										playerPosMatrix[playerTurn].y += 2;
										waitForInput = false;
									}
									else if (direction == RIGHT)
									{
										playerPosMatrix[playerTurn].x += 2;
										playerPosMatrix[playerTurn].y += 2;
										waitForInput = false;
									}
								}
								else if (pressedToMove(LEFT, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, playerPosMatrix[oppositePlayer(playerTurn, gameMode)], LEFT))
								{
									if (direction == UP)
									{
										playerPosMatrix[playerTurn].x -= 2;
										playerPosMatrix[playerTurn].y -= 2;
										waitForInput = false;
									}
									else if (direction == DOWN)
									{
										playerPosMatrix[playerTurn].x -= 2;
										playerPosMatrix[playerTurn].y += 2;
										waitForInput = false;
									}
								}
								else if (pressedToMove(RIGHT, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, playerPosMatrix[oppositePlayer(playerTurn, gameMode)], RIGHT))
								{
									if (direction == UP)
									{
										playerPosMatrix[playerTurn].x += 2;
										playerPosMatrix[playerTurn].y -= 2;
										waitForInput = false;
									}
									else if (direction == DOWN)
									{
										playerPosMatrix[playerTurn].x += 2;
										playerPosMatrix[playerTurn].y += 2;
										waitForInput = false;
									}
								}
								else unmovable = true;
							}
							else
							{
								Vector2i newPos = playerPosMatrix[playerTurn];
								switch (direction)
								{
								case LEFT:
								{
									newPos.x -= 2;
									if (pressedToMove(UP, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, newPos, UP))
									{
										newPos.y -= 2;
										if (freeSpace(playerPosMatrix, newPos, UP, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
										newPos.y += 2;
									}
									else if (pressedToMove(DOWN, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, newPos, DOWN))
									{
										newPos.y += 2;
										if (freeSpace(playerPosMatrix, newPos, DOWN, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
									}
									else unmovable = true;
									break;
								}
								case RIGHT:
								{
									newPos.x += 2;
									if (pressedToMove(UP, playerTurn, gameMode, event) && (isAbleToMove(boardMatrix, newPos, UP)))
									{
										newPos.y -= 2;
										if (freeSpace(playerPosMatrix, newPos, UP, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
										newPos.x -= 2;
									}
									else if (pressedToMove(DOWN, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, newPos, DOWN))
									{
										newPos.y += 2;
										if (freeSpace(playerPosMatrix, newPos, DOWN, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
									}
									else unmovable = true;
									break;
								}
								case UP:
								{
									newPos.y -= 2;
									if (pressedToMove(LEFT, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, newPos, LEFT))
									{
										newPos.x -= 2;
										if (freeSpace(playerPosMatrix, newPos, LEFT, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
										newPos.x += 2;
									}
									else if (pressedToMove(RIGHT, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, newPos, RIGHT))
									{
										newPos.x += 2;
										if (freeSpace(playerPosMatrix, newPos, RIGHT, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
									}
									else unmovable = true;
									break;
								}
								case DOWN:
								{
									newPos.y += 2;
									if (pressedToMove(LEFT, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, newPos, LEFT))
									{
										newPos.x -= 2;
										if (freeSpace(playerPosMatrix, newPos, LEFT, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
										newPos.x += 2;
									}
									else if (pressedToMove(RIGHT, playerTurn, gameMode, event) && isAbleToMove(boardMatrix, newPos, RIGHT))
									{
										newPos.x += 2;
										if (freeSpace(playerPosMatrix, newPos, RIGHT, FOURPLAYERS))
										{
											playerPosMatrix[playerTurn] = newPos;
											waitForInput = false;
											break;
										}
									}
									else unmovable = true;
									break;
								}
								}
							}
							if (waitForInput == false && unmovable == false)
							{
								changeGraphicalPos(Player[playerTurn], playerPosMatrix[playerTurn]);
								changePlayerTurn(playerTurn, gameMode);
							}
							else if (unmovable == true) waitForInput = false;
						}
						else
						{
							Vector2i playerPosMatrixOld = playerPosMatrix[playerTurn];
							if (wallBeingPlaced == false)
							{
								if (pressedToMove(LEFT, playerTurn, gameMode, event)) { direction = LEFT;  change = true; }
								else if (pressedToMove(RIGHT, playerTurn, gameMode, event)) { direction = RIGHT; change = true; }
								else if (pressedToMove(UP, playerTurn, gameMode, event)) { direction = UP;    change = true; }
								else if (pressedToMove(DOWN, playerTurn, gameMode, event)) { direction = DOWN;  change = true; }

								if (change == true)
								{
									if (gameMode == FOURPLAYERS && changeMatrixPos4Players(playerPosMatrix, playerTurn, boardMatrix, direction) == -1)
									{
										waitForInput = true;
									}
									else if (gameMode == TWOPLAYERS)
										waitForInput = modifyWaitForInput(gameMode, playerTurn, playerPosMatrix, boardMatrix, direction);
									if (waitForInput == false)
									{
										changeGraphicalPos(Player[playerTurn], playerPosMatrix[playerTurn]);
										if (playerPosMatrix[0].y == 0)
										{
											printf("Player 1 wins !");
											gameOver = true;
											gameOverFirstTime = true;
											winner = 0;
										}
										else if (playerPosMatrix[2].y == 16)
										{
											printf("Player 2 wins !");
											gameOver = true;
											gameOverFirstTime = true;
											winner = 2;
										}
										else if (gameMode == FOURPLAYERS)
										{
											if (playerPosMatrix[1].x == 16)
											{
												printf("Player 3 wins !");
												gameOver = true;
												gameOverFirstTime = true;
												winner = 1;
											}
											else if (playerPosMatrix[3].x == 0)
											{
												printf("Player 4 wins !");
												gameOver = true;
												gameOverFirstTime = true;
												winner = 3;
											}
										}
										if (playerPosMatrixOld.x != playerPosMatrix[playerTurn].x || playerPosMatrixOld.y != playerPosMatrix[playerTurn].y)
											changePlayerTurn(playerTurn, gameMode);
									}
									change = false;
								}
							}
						}
					}
					break;
				}
				case Event::MouseButtonReleased:
				{
					if (gameStarted == true)
					{
						Vector2i mousePos = Mouse::getPosition(gameWindow);
						if (wallBeingPlaced == true && mousePos.x >= 779 && mousePos.y >= 674 && mousePos.x <= 891 && mousePos.y <= 708)
						{
							if (event.mouseButton.button == Mouse::Left)
							{
								switch (playerTurn)
								{
								case 0:
								{
									Wall[playerTurn][wallIndex[playerTurn]].setPosition(762, 591);
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureH, 1);
									break;
								}
								case 2:
								{
									Wall[playerTurn][wallIndex[playerTurn]].setPosition(762, 283);
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureH, 1);
									break;
								}
								case 1:
								{
									Wall[playerTurn][wallIndex[playerTurn]].setPosition(762, 437);
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallGreenTextureH, 1);
									break;
								}
								case 3:
								{
									Wall[playerTurn][wallIndex[playerTurn]].setPosition(762, 129);
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallRedTextureH, 1);
									break;
								}
								}
								wallBeingPlaced = false;
								WallPlacement.setPosition(1000, 1000);
								WallPlacement.setTexture(WallPlacementTextureH);
							}
						}
						if (wallBeingPlaced == true && isBoardClicked(gameWindow, event))
						{
							if (event.mouseButton.button == Mouse::Left && isAllowedToPlaceWall(boardMatrix, WallPrediction, WallFacing))
							{
								int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
								int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
								int result = 0;
								int visited[17][17] = { 0 };
								bool copyBoardMatrix[17][17];
								for (int i = 0; i < 17; i++)
									for (int j = 0; j < 17; j++)
										copyBoardMatrix[i][j] = boardMatrix[i][j];
								createTemporaryWall(copyBoardMatrix, WallPrediction, WallFacing);
								if (gameMode == TWOPLAYERS)
								{
									searchRoute(copyBoardMatrix, playerPosMatrix[0].y, playerPosMatrix[0].x, 0, result, visited);
									if (result == 1)
									{
										emptyMatrix(visited);
										result = 0;
										searchRoute(copyBoardMatrix, playerPosMatrix[2].y, playerPosMatrix[2].x, 2, result, visited);
									}
								}
								else
								{
									searchRoute(copyBoardMatrix, playerPosMatrix[0].y, playerPosMatrix[0].x, 0, result, visited);
									if (result == 1)
									{
										emptyMatrix(visited);
										result = 0;
										searchRoute(copyBoardMatrix, playerPosMatrix[1].y, playerPosMatrix[1].x, 1, result, visited);
										if (result == 1)
										{
											emptyMatrix(visited);
											result = 0;
											searchRoute(copyBoardMatrix, playerPosMatrix[2].y, playerPosMatrix[2].x, 2, result, visited);
											if (result == 1)
											{
												emptyMatrix(visited);
												result = 0;
												searchRoute(copyBoardMatrix, playerPosMatrix[3].y, playerPosMatrix[3].x, 3, result, visited);
											}
										}
									}
								}
								for (int i = 0; i < 17; i++)
								{
									for (int j = 0; j < 17; j++)
										printf("%d ", visited[i][j]);
									printf("\n");
								}
								printf("%d", result);
								if (result == 1)
								{
									Wall[playerTurn][wallIndex[playerTurn]].setPosition(WallPrediction.x * 77 + 19 - WallWidth / 2, WallPrediction.y * 77 + 19 - WallHeight / 2);
									wallIndex[playerTurn]++;
									if (gameMode == TWOPLAYERS)
										amount = std::to_string(10 - wallIndex[playerTurn]);
									else amount = std::to_string(5 - wallIndex[playerTurn]);
									wallsLeft[playerTurn].setString(amount);
									changePlayerTurn(playerTurn, gameMode);
									wallBeingPlaced = false;
									WallPlacement.setPosition(1000, 1000);
									WallPlacement.setTexture(WallPlacementTextureH, 1);
									if (WallFacing == true)
									{
										boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 2] = true;
										boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 1] = true;
										boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2] = true;
									}
									else
									{
										boardMatrix[WallPrediction.y * 2 - 2][WallPrediction.x * 2 - 1] = true;
										boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 1] = true;
										boardMatrix[WallPrediction.y * 2][WallPrediction.x * 2 - 1] = true;
									}
								}
							}
						}
						else if (wallBeingPlaced == true && event.mouseButton.button == Mouse::Right)
						{
							if (WallFacing == true)
							{
								if (playerTurn == 0)
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureV, 1);
								else if (playerTurn == 1)
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallGreenTextureV, 1);
								else if (playerTurn == 2)
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureV, 1);
								else Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallRedTextureV, 1);
								WallFacing = false;
								WallPlacement.setTexture(WallPlacementTextureV, 1);
							}
							else
							{
								if (playerTurn == 0)
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallOrangeTextureH, 1);
								else if (playerTurn == 1)
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallGreenTextureH, 1);
								else if (playerTurn == 2)
									Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallBlueTextureH, 1);
								else Wall[playerTurn][wallIndex[playerTurn]].setTexture(WallRedTextureH, 1);
								WallFacing = true;
								WallPlacement.setTexture(WallPlacementTextureH, 1);
							}
							int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
							int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
							Vector2i mousePos = Mouse::getPosition(gameWindow);
							Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x - WallWidth / 2, mousePos.y - WallHeight / 2);
						}
					}
					else
					{
						Vector2i mousePos = Mouse::getPosition(gameWindow);
						if (event.mouseButton.button == Mouse::Left)
						{
							if (mousePos.x >= 225 && mousePos.y >= 238 && mousePos.x <= 724 && mousePos.y <= 302)
							{
								gameMode = TWOPLAYERS;
								gameStarted = true;
							}
							else if (mousePos.x >= 225 && mousePos.y >= 333 && mousePos.x <= 724 && mousePos.y <= 397)
							{
								gameMode = FOURPLAYERS;
								gameStarted = true;
							}
							else if (mousePos.x >= 725 && mousePos.y >= 617 && mousePos.x <= 924 && mousePos.y <= 681)
								gameWindow.close();
						}
					}
					break;
				}
				default:
				{
					if (gameStarted == true)
					{
						if (wallBeingPlaced == true)
						{
							int WallWidth = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().width;
							int WallHeight = Wall[playerTurn][wallIndex[playerTurn]].getGlobalBounds().height;
							Vector2i mousePos = Mouse::getPosition(gameWindow);
							Wall[playerTurn][wallIndex[playerTurn]].setPosition(mousePos.x - WallWidth / 2, mousePos.y - WallHeight / 2);
							wallPlacementPrediction(mousePos, WallPrediction);
							WallPlacement.setPosition(WallPrediction.x * 77 + 19 - WallWidth / 2, WallPrediction.y * 77 + 19 - WallHeight / 2);
						}
						else if (wallBeingPlaced == false && isSpriteClicked(Wall[playerTurn][wallIndex[playerTurn]], gameWindow))
						{
							wallBeingPlaced = true;
							WallFacing = true;
							if (gameMode == TWOPLAYERS)
								index = 10;
							else index = 5;
							if (playerTurn == 0 && wallIndex[0] + 1 < index)
								Wall[0][wallIndex[0] + 1].setPosition(762, 591);
							else if (playerTurn == 1 && wallIndex[1] + 1 < index)
								Wall[1][wallIndex[1] + 1].setPosition(762, 437);
							else if (playerTurn == 2 && wallIndex[2] + 1 < index)
								Wall[2][wallIndex[2] + 1].setPosition(762, 283);
							else if (playerTurn == 3 && wallIndex[3] + 1 < index)
								Wall[3][wallIndex[3] + 1].setPosition(762, 129);
						}
					}
					break;
				}
				}
			}
		}

		gameWindow.clear();
		gameWindow.draw(Board);
		gameWindow.draw(WallPlacement);
		if (gameMode == TWOPLAYERS)
		{
			for (index = 1; index < 10; index++)
			{
				gameWindow.draw(Wall[2][index]);
				gameWindow.draw(Wall[0][index]);
			}
		}
		else
		{
			for (index = 1; index < 5; index++)
			{
				gameWindow.draw(Wall[2][index]);
				gameWindow.draw(Wall[0][index]);
				gameWindow.draw(Wall[1][index]);
				gameWindow.draw(Wall[3][index]);
			}
		}
		gameWindow.draw(Wall[0][0]);
		gameWindow.draw(Wall[2][0]);
		if (gameMode == FOURPLAYERS)
		{
			gameWindow.draw(Wall[1][0]);
			gameWindow.draw(Wall[3][0]);
		}
		gameWindow.draw(Player[0]);
		gameWindow.draw(Player[2]);
		gameWindow.draw(wallsLeft[0]);
		gameWindow.draw(wallsLeft[2]);
		if (gameMode == FOURPLAYERS)
		{
			gameWindow.draw(Player[1]);
			gameWindow.draw(Player[3]);
			gameWindow.draw(wallsLeft[1]);
			gameWindow.draw(wallsLeft[3]);
		}
		gameWindow.display();
	}
	return 0;
}

void changeGraphicalPos(Sprite& player, Vector2i playerPosMatrix)
{
	player.setPosition((playerPosMatrix.x / 2) * JUMP + 13, (playerPosMatrix.y / 2) * JUMP + 10);
}

bool changeMatrixPos(Vector2i &playerPosMatrix, Vector2i player2PosMatrix, bool boardMatrix[17][17], int direction)
{
	Vector2i nextPos = playerPosMatrix;
	if (isAbleToMove(boardMatrix, playerPosMatrix, direction))
	{
		switch (direction)
		{
		case LEFT: {nextPos.x -= 2; break; }
		case RIGHT: {nextPos.x += 2; break; }
		case UP: {nextPos.y -= 2; break; }
		case DOWN: {nextPos.y += 2; break; }
		}
		if (nextPos == player2PosMatrix && isAbleToMove(boardMatrix, nextPos, direction))
		{
			switch (direction)
			{
			case LEFT: {playerPosMatrix.x -= 4; break; }
			case RIGHT: {playerPosMatrix.x += 4; break; }
			case UP: {playerPosMatrix.y -= 4; break; }
			case DOWN: {playerPosMatrix.y += 4; break; }
			}
		}
		else if (nextPos != player2PosMatrix)
		{
			switch (direction)
			{
			case LEFT: {playerPosMatrix.x -= 2; break; }
			case RIGHT: {playerPosMatrix.x += 2; break; }
			case UP: {playerPosMatrix.y -= 2; break; }
			case DOWN: {playerPosMatrix.y += 2; break; }
			}
		}
		else return true;
	}
	return false;
}

int changeMatrixPos4Players(Vector2i playerPosMatrix[4], short playerTurn, bool boardMatrix[17][17], int direction)
{
	Vector2i nextPos = playerPosMatrix[playerTurn];
	switch (direction)
	{
	case LEFT: {nextPos.x -= 2; break; }
	case RIGHT: {nextPos.x += 2; break; }
	case UP: {nextPos.y -= 2; break; }
	case DOWN: {nextPos.y += 2; break; }
	}
	if (isAbleToMove(boardMatrix, playerPosMatrix[playerTurn], direction))
	{

		if (freeSpace(playerPosMatrix, nextPos, direction, FOURPLAYERS))
		{
			playerPosMatrix[playerTurn] = nextPos;
			return 1;
		}
		else
		{
			Vector2i tempPos = nextPos;
			switch (direction)
			{
			case LEFT: {nextPos.x -= 2; break; }
			case RIGHT: {nextPos.x += 2; break; }
			case UP: {nextPos.y -= 2; break; }
			case DOWN: {nextPos.y += 2; break; }
			}
			if (isAbleToMove(boardMatrix, tempPos, direction) && freeSpace(playerPosMatrix, nextPos, direction, FOURPLAYERS))
			{
				playerPosMatrix[playerTurn] = nextPos;
				return 1;
			}
			else if (!isAbleToMove(boardMatrix, tempPos, direction))
				return -1;
			else return 0;
		}
	}
}

bool isSpriteClicked(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width
		&& mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
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
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left
		&& mousePos.x >= 26 && mousePos.x <= 705 && mousePos.y >= 26 && mousePos.y <= 705)
	{
		printf("Board Clicked\n");
		return true;
	}
	return false;
}

bool isInsideWall(Sprite Wall, RenderWindow &Window)
{
	Vector2i mousePos = Mouse::getPosition(Window);
	if (mousePos.x > Wall.getPosition().x && mousePos.x < Wall.getPosition().x + Wall.getGlobalBounds().width
		&& mousePos.y > Wall.getPosition().y && mousePos.y < Wall.getPosition().y + Wall.getGlobalBounds().height)
		return true;
	return false;
}

bool isAbleToMove(bool boardMatrix[17][17], Vector2i playerPos, int direction)
{
	switch (direction)
	{
	case LEFT: { return boardMatrix[playerPos.y][playerPos.x - 1] == false && playerPos.x - 2 >= 0; break; }
	case RIGHT: { return boardMatrix[playerPos.y][playerPos.x + 1] == false && playerPos.x + 2 <= 17; break; }
	case UP: { return boardMatrix[playerPos.y - 1][playerPos.x] == false && playerPos.y - 2 >= 0; break; }
	case DOWN: { return boardMatrix[playerPos.y + 1][playerPos.x] == false && playerPos.y + 2 <= 17; break; }
	default: { return false; break; }
	}
}

void wallPlacementPrediction(Vector2i mousePos, Vector2i &prediction)
{
	prediction.x = (mousePos.x + 19) / 77;
	prediction.y = (mousePos.y + 19) / 77;
	if (prediction.x < 1) prediction.x = 1;
	if (prediction.x > 8) prediction.x = 8;
	if (prediction.y < 1) prediction.y = 1;
	if (prediction.y > 8) prediction.y = 8;

}

bool isAllowedToPlaceWall(bool boardMatrix[17][17], Vector2i prediction, bool wallFacing)
{
	if (wallFacing == true)
	{
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2 - 2] == true)
			return false;
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2 - 1] == true)
			return false;
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2] == true)
			return false;
	}
	else
	{
		if (boardMatrix[prediction.y * 2 - 2][prediction.x * 2 - 1] == true)
			return false;
		if (boardMatrix[prediction.y * 2 - 1][prediction.x * 2 - 1] == true)
			return false;
		if (boardMatrix[prediction.y * 2][prediction.x * 2 - 1] == true)
			return false;
	}
	return true;
}

bool pressedToMove(int direction, short playerTurn, bool gameMode, Event event)
{
	switch (direction)
	{
	case LEFT:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Left && ((gameMode == TWOPLAYERS && playerTurn == 0) || gameMode == FOURPLAYERS))
			|| (Event::KeyReleased && event.key.code == Keyboard::A && (playerTurn == 2 && gameMode == TWOPLAYERS)));
		break;
	}
	case RIGHT:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Right && ((gameMode == TWOPLAYERS && playerTurn == 0) || gameMode == FOURPLAYERS))
			|| (Event::KeyReleased && event.key.code == Keyboard::D && (playerTurn == 2 && gameMode == TWOPLAYERS)));
		break;
	}
	case UP:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Up && ((gameMode == TWOPLAYERS && playerTurn == 0) || gameMode == FOURPLAYERS))
			|| (Event::KeyReleased && event.key.code == Keyboard::W && (playerTurn == 2 && gameMode == TWOPLAYERS)));
		break;
	}
	case DOWN:
	{
		return ((Event::KeyReleased && event.key.code == Keyboard::Down && ((gameMode == TWOPLAYERS && playerTurn == 0) || gameMode == FOURPLAYERS))
			|| (Event::KeyReleased && event.key.code == Keyboard::S && (playerTurn == 2 && gameMode == TWOPLAYERS)));
		break;
	}
	default: { return false; break; }
	}
}

void searchRoute(bool boardMatrix[17][17], int playerY, int playerX, short playerTurn, int &result, int visited[17][17])
{
	if (result == 0)
	{
		switch (playerTurn)
		{
		case 0: {if (playerY == 0)  result = 1; break; }
		case 2: {if (playerY == 16) result = 1; break; }
		case 3: {if (playerX == 0)  result = 1; break; }
		case 1: {if (playerX == 16) result = 1; break; }
		}
		if (visited[playerY][playerX] == 0 && result == 0)
		{
			visited[playerY][playerX] = 1;
			if (boardMatrix[playerY - 1][playerX] == 0 && isInBorders(playerY - 2, playerX))
				searchRoute(boardMatrix, playerY - 2, playerX, playerTurn, result, visited);
			if (boardMatrix[playerY][playerX - 1] == 0 && isInBorders(playerY, playerX - 2))
				searchRoute(boardMatrix, playerY, playerX - 2, playerTurn, result, visited);
			if (boardMatrix[playerY][playerX + 1] == 0 && isInBorders(playerY, playerX + 2))
				searchRoute(boardMatrix, playerY, playerX + 2, playerTurn, result, visited);
			if (boardMatrix[playerY + 1][playerX] == 0 && isInBorders(playerY + 2, playerX))
				searchRoute(boardMatrix, playerY + 2, playerX, playerTurn, result, visited);
		}
	}
}

bool isInBorders(int playerY, int playerX)
{
	return playerY < 17 && playerX < 17 && playerY >= 0 && playerX >= 0;
}

void createTemporaryWall(bool boardMatrix[17][17], Vector2i WallPrediction, bool WallFacing)
{
	if (WallFacing == true)
	{
		boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 2] = true;
		boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 1] = true;
		boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2] = true;
	}
	else
	{
		boardMatrix[WallPrediction.y * 2 - 2][WallPrediction.x * 2 - 1] = true;
		boardMatrix[WallPrediction.y * 2 - 1][WallPrediction.x * 2 - 1] = true;
		boardMatrix[WallPrediction.y * 2][WallPrediction.x * 2 - 1] = true;
	}
}

void changePlayerTurn(short &playerTurn, bool gameMode)
{
	if (gameMode == TWOPLAYERS)
	{
		if (playerTurn == 0)
			playerTurn = 2;
		else playerTurn = 0;
	}
	else
	{
		++playerTurn;
		if (playerTurn == 4)
			playerTurn = 0;
	}
}

void emptyMatrix(int matrix[17][17])
{
	for (int i = 0; i < 17; i++)
		for (int j = 0; j < 17; j++)
			matrix[i][j] = 0;
}

int oppositePlayer(short playerTurn, bool gameMode)
{
	if (gameMode == TWOPLAYERS)
	{
		if (playerTurn == 0)
			return 2;
		else return 0;
	}
	else
	{
		if (playerTurn == 3)
			return 0;
		else return playerTurn + 1;
	}
}

bool modifyWaitForInput(bool gameMode, short playerTurn, Vector2i playerPosMatrix[4], bool boardMatrix[17][17], int direction)
{
	Vector2i playerPosMatrixOld = playerPosMatrix[playerTurn];
	if (gameMode == TWOPLAYERS)
	{
		if (playerTurn == 0)
		{
			if (changeMatrixPos(playerPosMatrix[0], playerPosMatrix[2], boardMatrix, direction))
				return true;
		}
		else
		{
			if (changeMatrixPos(playerPosMatrix[2], playerPosMatrix[0], boardMatrix, direction))
				return true;
		}
	}
	else if (gameMode == FOURPLAYERS)
	{
		if (playerTurn == 0)
		{
			if (changeMatrixPos(playerPosMatrix[0], playerPosMatrix[1], boardMatrix, direction))
				return true;
			else
			{
				playerPosMatrix[0] = playerPosMatrixOld;
				if (changeMatrixPos(playerPosMatrix[0], playerPosMatrix[2], boardMatrix, direction))
					return true;
				else
				{
					playerPosMatrix[0] = playerPosMatrixOld;
					if (changeMatrixPos(playerPosMatrix[0], playerPosMatrix[3], boardMatrix, direction))
						return true;
				}
			}
		}
		else if (playerTurn == 1)
		{
			if (changeMatrixPos(playerPosMatrix[1], playerPosMatrix[0], boardMatrix, direction))
				return true;
			else
			{
				playerPosMatrix[1] = playerPosMatrixOld;
				if (changeMatrixPos(playerPosMatrix[1], playerPosMatrix[2], boardMatrix, direction))
					return true;
				else
				{
					playerPosMatrix[1] = playerPosMatrixOld;
					if (changeMatrixPos(playerPosMatrix[1], playerPosMatrix[3], boardMatrix, direction))
						return true;
				}
			}
		}
		else if (playerTurn == 2)
		{
			if (changeMatrixPos(playerPosMatrix[2], playerPosMatrix[0], boardMatrix, direction))
				return true;
			else
			{
				playerPosMatrix[2] = playerPosMatrixOld;
				if (changeMatrixPos(playerPosMatrix[2], playerPosMatrix[1], boardMatrix, direction))
					return true;
				else
				{
					playerPosMatrix[2] = playerPosMatrixOld;
					if (changeMatrixPos(playerPosMatrix[2], playerPosMatrix[3], boardMatrix, direction))
						return true;
				}
			}
		}
		else if (playerTurn == 3)
		{
			if (changeMatrixPos(playerPosMatrix[3], playerPosMatrix[0], boardMatrix, direction))
				return true;
			else
			{
				playerPosMatrix[3] = playerPosMatrixOld;
				if (changeMatrixPos(playerPosMatrix[3], playerPosMatrix[1], boardMatrix, direction))
					return true;
				else
				{
					playerPosMatrix[3] = playerPosMatrixOld;
					if (changeMatrixPos(playerPosMatrix[3], playerPosMatrix[2], boardMatrix, direction))
						return true;
				}
			}
		}
	}
	return false;
}


bool freeSpace(Vector2i playerPosMatrix[4], Vector2i nextPos, int direction, bool gameMode)
{
	if (gameMode == TWOPLAYERS)
	{
		if (playerPosMatrix[0].x == nextPos.x && playerPosMatrix[0].y == nextPos.y)
			return false;
		if (playerPosMatrix[2].x == nextPos.x && playerPosMatrix[2].y == nextPos.y)
			return false;
	}
	else
	{
		if (playerPosMatrix[0].x == nextPos.x && playerPosMatrix[0].y == nextPos.y)
			return false;
		if (playerPosMatrix[1].x == nextPos.x && playerPosMatrix[1].y == nextPos.y)
			return false;
		if (playerPosMatrix[2].x == nextPos.x && playerPosMatrix[2].y == nextPos.y)
			return false;
		if (playerPosMatrix[3].x == nextPos.x && playerPosMatrix[3].y == nextPos.y)
			return false;
	}
	return true;
}