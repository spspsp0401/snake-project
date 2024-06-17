#ifndef __SNAKE_H__
#define __SNAKE_H__
#include <vector>
#include <utility>
#include "MapManager.h"
using namespace std;



class Snake {
    int x, y;
    pair<int, int> lastPos;

    int len;
    int growCnt;
    int poisCnt;
    int gateCnt;
    int dir;

    vector<pair<int,int> > bodies;
public:
    
    Snake() {}
    Snake(int x, int y);
    Snake(pair<int,int> pos);
    
    void updateSnake(); 

    int getX();
    int getY();
    pair<int,int> getXYPair();
    int getLen();
    int getGrowCnt();
    int getPoisCnt();
    int getGateCnt();
    
    vector<pair<int,int> > getBodies(); 

    bool setDir(int d); 

    bool decreaseBody();
    void growBody();

    void inGate(MapManager *MapMan);

    bool conflictCheck();
};
#endif
