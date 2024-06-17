#include "Snake.h"

int dx[] = {1,0,-1,0};
int dy[] = {0,-1,0,1};
// 0 == Down, 1 == Left, 2 == Up, 3 == Right;

Snake::Snake(int x, int y) : x(x), y(y) {
  for(int i = 1; i <= 2; i++) {
    bodies.push_back(make_pair(x, y + i));
  }
  dir = 1;
  lastPos = make_pair(x, y + 3);
  len = 3;
  growCnt = 0;
  poisCnt = 0;
  gateCnt = 0;
}

Snake::Snake(pair<int,int> pos) {
  x = pos.first;
  y = pos.second;
  for(int i = 1; i <= 2; i++) {
    bodies.push_back(make_pair(x, y + i));
  }
  dir = 1;
  lastPos = make_pair(x, y + 3);
  len = 3;
  growCnt = 0;
  poisCnt = 0;
  gateCnt = 0;
}

void Snake::updateSnake() {
  lastPos = bodies.back();
  for(int i = (int)(bodies.size()) - 1; i > 0; i--) {
    bodies[i] = bodies[i - 1];
  }
  bodies[0] = make_pair(x, y);
  x += dx[dir];
  y += dy[dir];
}


int Snake::getX() {
  return x;
}
int Snake::getY() {
  return y;
}
pair<int,int> Snake::getXYPair() {
  return make_pair(x, y);
}
int Snake::getLen() {
  return len;
}

int Snake::getGrowCnt() {
  return growCnt;
}

int Snake::getPoisCnt() {
  return poisCnt;
}

int Snake::getGateCnt() {
  return gateCnt;
}

vector<pair<int,int> > Snake::getBodies() {
  return bodies;
}

bool Snake::decreaseBody() {
  len -= 1;
  this->bodies.pop_back();
  if(len < 3) return false;
  poisCnt += 1;
  return true;
}

void Snake::growBody() {
  growCnt += 1;
  this->bodies.push_back(this->lastPos);
  len += 1;
}

void Snake::inGate(MapManager *MapMan) {
  gateCnt += 1;
  int **CurMap = MapMan->getMap();
  if(x == MapMan->gateX[0] && y == MapMan->gateY[0]) {
    x = MapMan->gateX[1];
    y = MapMan->gateY[1];
  } else if(x == MapMan->gateX[1] && y == MapMan->gateY[1]) {
    x = MapMan->gateX[0];
    y = MapMan->gateY[0];
  }

  if(x == 0) {
    dir = 0;  
  } else if(x == 31) {
    dir = 2;
  } else if(y == 0) {
    dir = 3;
  } else if(y == 31) {
    dir = 1;
  } else {
    int nd[4] = {dir, (dir + 1) % 4, (dir - 1) % 4, (dir + 2) % 4};
    for(int i = 0; i < 4; i++) {
      int nx = x + dx[nd[i]];
      int ny = y + dy[nd[i]];
      if(nx < 0 || nx >= 32 || ny < 0 || ny >= 32) continue;
      if(CurMap[nx][ny] == 0) {
        dir = nd[i];
        break;
      }
    }
  }
}

bool Snake::setDir(int d) {
  if(d == ((dir + 2) % 4)) return false;
  dir = d;
  return true;
}