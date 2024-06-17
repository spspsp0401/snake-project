#ifndef __MAPMAN_H__
#define __MAPMAN_H__
#include <utility>
#include <ctime>
#include <cstdlib>
#include <vector>
using namespace std;



class MapManager {
private:
    pair<int,int> initPos;
    int **CurMap;
    int StageCnt;
    int growX[2];
    int growY[2];
    int poisX;
    int poisY;
    

public:
    int gateX[2];
    int gateY[2];
    MapManager(int StageCnt);
    int **getMap() {return CurMap;}
    pair<int,int> getInitPos() {return initPos;}
    void groPoisReset();
    void groPoisSet(int x, int y);
    void gateSet();
    void gateReset();
    vector<pair<int,int>> getGateXY() {
        vector<pair<int,int>> tmp = {make_pair(gateX[0], gateY[0]), make_pair(gateX[1], gateY[1])};
        return tmp;
    }
};
#endif
