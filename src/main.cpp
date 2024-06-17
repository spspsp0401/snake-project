#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <clocale>
#include "Snake.h"
using namespace std;

bool inGate(Snake& snake, vector<pair<int,int>> gateXY) {
  vector<pair<int,int>> bodies = snake.getBodies();
  for(int i = 0; i < 2; i++) {
    int dist = abs(snake.getX() - gateXY[i].first) + abs(snake.getY() - gateXY[i].second);
    if(dist <= 4) return true;
    for(pair<int,int> &p : bodies) { 
      dist = abs(p.first - gateXY[i].first) + abs(p.second - gateXY[i].second);
      if(dist <= 1) return true;
    }
  }
  return false;
}


bool kbhit()
{
  int ch;
  bool ret;

  nodelay(stdscr, TRUE);

  ch = getch();
  if (ch == ERR) {
    ret = false;
  } else {
    ret = true;
    ungetch(ch);
  }

  nodelay(stdscr, FALSE);
  return ret;
}



int main() {

  initscr();
  curs_set(0);
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  resize_term(34, 95);
  border('|','|','-','-','+','+','+','+');
  mvprintw(1,44,"SNAKE GAME");
  refresh();
  init_pair(1,COLOR_WHITE,COLOR_BLACK);
  WINDOW *gameWin;
  WINDOW *scoreWin;
  WINDOW *missionWin;


  gameWin = newwin(32,32,1,2);
  wbkgd(gameWin, COLOR_PAIR(1));
  wattron(gameWin, COLOR_PAIR(1));
  wborder(gameWin,'+','+','+','+','#','#','#','#');
  wrefresh(gameWin);
  

  scoreWin = newwin(14,30,1,63);
  wbkgd(scoreWin, COLOR_PAIR(1));
  wattron(scoreWin, COLOR_PAIR(1));
  wborder(scoreWin, '|', '|', '-', '-', '#', '#', '#', '#');
  mvwprintw(scoreWin, 1, 9, "SCORE BOARD");
  wrefresh(scoreWin);


  missionWin = newwin(17,30,16,63);
  wbkgd(missionWin, COLOR_PAIR(1));
  wattron(missionWin, COLOR_PAIR(1));
  wborder(missionWin, '|', '|', '-', '-', '#', '#', '#', '#');
  mvwprintw(missionWin, 1, 8, "MISSION BOARD");
  wrefresh(missionWin);



  int StageCnt = 0;
  bool isGameOver = false;
  while(StageCnt < 3 && !isGameOver) {
    
    mvprintw(3,42, "STAGE NUM : #%d",StageCnt + 1);
    mvprintw(5,38,"PRESS ANY KEY TO START");
    refresh();
    getch();
    mvprintw(3,42, "                     ");
    mvprintw(5,38, "                        ");
    refresh();

    MapManager MapMan(StageCnt);
    Snake snake(MapMan.getInitPos());
    MapMan.groPoisSet(snake.getX(), snake.getY());
    MapMan.gateSet();
    int **CurMap = MapMan.getMap();
    int turn = 1;
    bool missionFlag[4] = {false, };
    while(!isGameOver && turn++) {
    
      if(turn > 85) {
        MapMan.groPoisReset();
        MapMan.groPoisSet(snake.getX(),snake.getY());
        if(!inGate(snake,MapMan.getGateXY())) {
          MapMan.gateReset();
          MapMan.gateSet();
        }
        CurMap = MapMan.getMap();
        turn = 1;
      }
      
      int nextDir = -1;
      
      long int refTime = clock();
      long int currentTime = 0;
      while (1)
      {
        if (kbhit())
        {
          int ch = getch();
          switch (ch) {
          case KEY_RIGHT:
            nextDir = 3;
            break;
          case KEY_LEFT:
            nextDir = 1;
            break;
          case KEY_DOWN:
            nextDir = 0;
            break;
          case KEY_UP:
            nextDir = 2;
            break;
          default:
            break;
          }
          if(nextDir == -1)
            continue;
          
          if (snake.setDir(nextDir) != true)
          {
            isGameOver = true;
          }
        }
        
        currentTime += clock() - refTime;
        if (currentTime > clock_t(10000000000)) break;
      }

      if(isGameOver) break;

      
      snake.updateSnake();
      int &headPos = CurMap[snake.getX()][snake.getY()];
      if(headPos == 1 || headPos == 2) {
        isGameOver = true;
        break;
      } else if(headPos == 3) {
        snake.growBody();
        headPos = 0;
      } else if (headPos == 4) {
        if(!snake.decreaseBody()) {
          isGameOver = true;
          break;
        }
        headPos = 0;
      } else if(headPos == 5) {
        snake.inGate(&MapMan);
      }
      

      vector<pair<int,int> > bodies = snake.getBodies();
      for(int i = 0; i < bodies.size(); i++) { 
        const int &Bx = bodies[i].first;
        const int &By = bodies[i].second;
        if(Bx == snake.getX() && By == snake.getY()) {
          isGameOver = true;
          break;
        }
      }


      wborder(gameWin,'+','+','+','+','#','#','#','#');
      for (int i = 1; i < 31; i++)
        for (int j = 1; j < 31; j++) {
          mvwprintw(gameWin, i, j, " ");
          if(CurMap[i][j] == 1 || CurMap[i][j] == 2) {
            mvwprintw(gameWin, i, j, "+");
          } else if(CurMap[i][j] == 3) {
            mvwprintw(gameWin, i, j, "O");
          } else if(CurMap[i][j] == 4) {
            mvwprintw(gameWin, i, j, "X");
          }
        }
      for(int i = 0; i < 2; i++) {
        mvwprintw(gameWin, MapMan.gateX[i], MapMan.gateY[i], "W");
      }
          

      

      mvwprintw(gameWin, snake.getX(), snake.getY(), "@");
      for (int i = 0; i < bodies.size(); i++)
      {
        const pair<int,int> &p = bodies[i];
        mvwprintw(gameWin, p.first, p.second, "O");
      }
      wrefresh(gameWin);

      mvwprintw(scoreWin, 4, 9, "Size : %lu / (15)", bodies.size() + 1);
      mvwprintw(scoreWin, 6, 9, "Grow : %d", snake.getGrowCnt());
      mvwprintw(scoreWin, 8, 9, "Pois : %d", snake.getPoisCnt());
      mvwprintw(scoreWin, 10, 9, "Gate : %d", snake.getGateCnt());
      wrefresh(scoreWin);

      if(bodies.size() + 1 >= 6) missionFlag[0] = true;
      if(snake.getGrowCnt() >= 6) missionFlag[1] = true;
      if(snake.getPoisCnt() >= 3) missionFlag[2] = true;
      if(snake.getGateCnt() >= 2) missionFlag[3] = true;
      mvwprintw(missionWin, 4, 9, "Size : 6 (%c)", missionFlag[0] ? 'V' : ' ');
      mvwprintw(missionWin, 7, 9, "Grow : 6 (%c)", missionFlag[1] ? 'V' : ' ');
      mvwprintw(missionWin, 10, 9, "Pois : 3 (%c)", missionFlag[2] ? 'V' : ' ');
      mvwprintw(missionWin, 13, 9, "Gate : 2 (%c)", missionFlag[3] ? 'V' : ' ');
      wrefresh(missionWin);
      int cnt = 0;
      for(int i = 0; i < 4; i++) {
        cnt += missionFlag[i];
      }
      if(cnt == 4) {
        StageCnt += 1;
        break;
      }
    }
  }
  return 0;
}