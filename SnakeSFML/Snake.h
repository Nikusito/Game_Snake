#pragma once
enum EDirection {
  Left,
  Right,
  Down,
  Up
};

enum class ESegment {
  Head,
  Left_Right,
  Up_Down,
  Up_Right,
  Up_Left,
  Down_Left,
  Down_Right,
  Tail,
  SnakeSprite
};

struct SnakeSegment
{// TODO: Add constructor with "row, col, direction" arguments that set corresponding class members. Default value for direction should be EDirection::Left
  SnakeSegment(int _row = 0, int _col = 0, int _rotat = 0, EDirection _direction = EDirection::Left, ESegment _segment = ESegment::SnakeSprite);

 //Draw Sprite
 void Draw();

  //Moved Sprite head
 void MoveDown(int& _deltacol, int& _deltarow, sf::Sprite& sprite);
 void MoveUp(int& _deltacol, int& _deltarow, sf::Sprite& sprite);
 void MoveLeft(int& _deltacol, int& _deltarow, sf::Sprite& sprite);
 void MoveRight(int& _deltacol, int& _deltarow, sf::Sprite& sprite);

 void Move();

  ESegment segment;
  EDirection direction;

  int col;
  int row;
  int rotation;
};
