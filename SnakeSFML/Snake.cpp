#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Snake.h"


SnakeSegment::SnakeSegment(int _row = 0, int _col = 0, int _rotat = 0, EDirection _direction = EDirection::Left, ESegment _segment = ESegment::SnakeSprite) :
  row(_row),
  col(_col),
  rotation(_rotat),
  direction(_direction),
  segment(_segment)
{}

void SnakeSegment::Draw()
{
  switch (segment)
  {
  case ESegment::Head:
    DrawSprite(headSprite, col, row);
    break;
  case ESegment::Left_Right:
    LeftRightSprite.setOrigin(0.f, 0.f);
    DrawSprite(LeftRightSprite, col, row);
    break;
  case ESegment::Up_Down:
    DrawSprite(UpDownSprite, col, row);
    break;
  case ESegment::Up_Right:
    DrawSprite(UpRightSprite, col, row);
    break;
  case ESegment::Up_Left:
    DrawSprite(UpLeftSprite, col, row);
    break;
  case ESegment::Down_Left:
    DrawSprite(DownLeftSprite, col, row);
    break;
  case ESegment::Down_Right:
    DrawSprite(DownRightSprite, col, row);
    break;
  case ESegment::Tail:
    DrawSprite(tailSprite, col, row);
    break;
  case ESegment::SnakeSprite:
    DrawSprite(snakeSprite, col, row);
    break;
  }
}

void SnakeSegment::MoveDown(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
  _deltacol = 0;
  _deltarow = 1;
  sprite.setOrigin(0.f, 0.f);
  sprite.setRotation(0.f);
}
void SnakeSegment::MoveUp(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
  _deltacol = 0;
  _deltarow = -1;
  sprite.setOrigin(64.f, 64.f);
  sprite.setRotation(180.f);
}
void SnakeSegment::MoveLeft(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
  _deltacol = -1;
  _deltarow = 0;
  sprite.setOrigin(0.f, 64.f);
  sprite.setRotation(90.f);
}
void SnakeSegment::MoveRight(int& _deltacol, int& _deltarow, sf::Sprite& sprite) {
  _deltacol = 1;
  _deltarow = 0;
  sprite.setOrigin(64.f, 0.f);
  sprite.setRotation(-90.f);
}

void SnakeSegment::Move()
{
  int deltaCol = -1;
  int deltaRow = 0;

  switch (direction)
  {
  case Down:
  {
    MoveDown(deltaCol, deltaRow, headSprite);
    break;
  }
  case Up:
  {
    MoveUp(deltaCol, deltaRow, headSprite);
    break;
  }
  case Left:
  {
    MoveLeft(deltaCol, deltaRow, headSprite);
    break;
  }
  case Right:
  {
    MoveRight(deltaCol, deltaRow, headSprite);
    break;
  }
  }

  col += deltaCol;
  row += deltaRow;
}
