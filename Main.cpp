//
//  main.cpp
//  Project 3
//
//  Created by Aidan Cone on 5/19/23.
//
#include "Board.h"
#include "Side.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <cassert>
using namespace std;

void doGameTests()
{
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 0);
    b.setBeans(SOUTH, 1, 2);
    b.setBeans(NORTH, 2, 1);
    b.setBeans(NORTH, 3, 2);
    Game g(b, &bp1, &bp2);
    bool over;
    bool hasWinner;
    Side winner;
    //    Homer
    //   0  1  2
    // 0         0
    //   2  0  0
    //    Bart
    g.status(over, hasWinner, winner);
    assert(!over);
    assert(g.beans(NORTH, POT) == 0);
    assert(g.beans(SOUTH, POT) == 0 );
    assert(g.beans(NORTH, 1) == 0);
    assert(g.beans(NORTH, 2) == 1);
    assert(g.beans(NORTH, 3) == 2);
    assert(g.beans(SOUTH, 1) == 2);
    assert(g.beans(SOUTH, 2) == 0);
    assert(g.beans(SOUTH, 3) == 0);
    g.move(SOUTH);
    //   0  1  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over);
    assert(g.beans(NORTH, POT) == 0);
    assert(g.beans(SOUTH, POT) == 3);
    assert(g.beans(NORTH, 1) == 0);
    assert(g.beans(NORTH, 2) == 1);
    assert(g.beans(NORTH, 3) == 0);
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 1);
    assert(g.beans(SOUTH, 3) == 0);
    
    g.move(NORTH);
    //   1  0  0
    // 0         3
    //   0  1  0
    g.status(over, hasWinner, winner);
    assert(!over);
    assert(g.beans(NORTH, POT) == 0);
    assert(g.beans(SOUTH, POT) == 3);
    assert(g.beans(NORTH, 1) == 1);
    assert(g.beans(NORTH, 2) == 0);
    assert(g.beans(NORTH, 3) == 0);
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 1);
    assert(g.beans(SOUTH, 3) == 0);
    
    g.move(SOUTH);
    g.display();
    //   1  0  0
    // 0         3
    //   0  0  1
    g.status(over, hasWinner, winner);
    assert(!over);
    assert(g.beans(NORTH, POT) == 0);
    assert(g.beans(SOUTH, POT) == 3);
    assert(g.beans(NORTH, 1) == 1);
    assert(g.beans(NORTH, 2) == 0);
    assert(g.beans(NORTH, 3) == 0);
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 0);
    assert(g.beans(SOUTH, 3) == 1);
    
    g.move(NORTH);
    //   0  0  0
    // 1         4
    //   0  0  0
    g.status(over, hasWinner, winner);
    assert(over);
    assert(g.beans(NORTH, POT) == 1);
    assert(g.beans(SOUTH, POT) == 4);
    assert(g.beans(NORTH, 1) == 0);
    assert(g.beans(NORTH, 2) == 0);
    assert(g.beans(NORTH, 3) == 0);
    assert(g.beans(SOUTH, 1) == 0);
    assert(g.beans(SOUTH, 2) == 0);
    assert(g.beans(SOUTH, 3) == 0);
    assert(hasWinner && winner == SOUTH);
}

void doBoardTests()
{
    Board b(3, 2);
    assert(b.holes() == 3);
    assert( b.totalBeans() == 12);
    assert(b.beans(SOUTH, POT) == 0);
    assert(b.beansInPlay(SOUTH) == 6);
    b.setBeans(SOUTH, 1, 1);
    b.moveToPot(SOUTH, 2, SOUTH);
    assert(b.totalBeans() == 11);
    assert(b.beans(SOUTH, 1) == 1 );
    assert(b.beans(SOUTH, 2) == 0);
    assert(b.beans(SOUTH, POT) == 2);
    assert(b.beansInPlay(SOUTH) == 3);
    Side es;
    int eh;
    b.sow(SOUTH, 3, es, eh);
    assert(es == NORTH);
    assert(eh == 3);
    assert(b.beans(SOUTH, 3) == 0);
    assert(b.beans(NORTH, 3) == 3);
    assert(b.beans(SOUTH, POT) == 3);
    assert(b.beansInPlay(SOUTH) == 1);
    assert(b.beansInPlay(NORTH) == 7);
    
    Board c(4,2);
    
    assert(c.holes() == 4);
    assert(c.totalBeans() == 16);
    assert(c.beans(SOUTH, POT) == 0);
    assert(c.beans(NORTH, POT) == 0);
    assert(c.beansInPlay(SOUTH) == 8);
    assert(c.beansInPlay(NORTH) == 8);
    c.setBeans(NORTH, 1, 3);
    c.moveToPot(NORTH, 2, NORTH);
    Side es1;
    int eh1;
    c.sow(NORTH, 1, es1, eh1);
    assert(es1 == SOUTH);
    assert(eh1 == 2);
    assert(c.beans(NORTH, POT) == 3);
    assert(c.beans(SOUTH, 1) == 3);
    assert(c.beansInPlay(NORTH) == 4);
    assert(c.beansInPlay(SOUTH) == 10);
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Board b(3, 2);
    b.setBeans(SOUTH, 2, 0);
    cout << "=========" << endl;
    int n = hp.chooseMove(b, SOUTH);
    cout << "=========" << endl;
    assert(n == 1  ||  n == 3);
    n = bp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    n = sp.chooseMove(b, SOUTH);
    assert(n == 1  ||  n == 3);
    
}

int main()
{
    doBoardTests();  // WORKS ON g32
    doPlayerTests(); // WORKS ON g32
    doGameTests();   // WORKS ON g32
//
//----------------------------------HumanPlayer vs SmartPlayer----------------------------------------------------------

//    Board b(3,3);
//    HumanPlayer h1("Aidan"); // makes one move for human and then does nothing for smart player
//    SmartPlayer s1("Smart Player");
//    Game g1(b, &s1, &h1);
//    g1.play();
//
    //================================= WORKS CORRECTLY ===================================================
    
//------------------------------------HumanPlayer vs BadPlayer---------------------------------------------------------

//    Board l(3,3);
//    HumanPlayer h2("Aidan");
//    BadPlayer b2("Bad Player");
//    Game g2(l, &b2, &h2);
//    g2.play();
    
    //================================== WORKS CORRECTLY =============================================================
    
//----------------------------------------HumanPlayer vs HumanPlayer-------------------------------------------------
//    Board hvh(6,3);
//    HumanPlayer hvh1("Aidan");
//    HumanPlayer hvh2("Shaley");
//    Game gHvH(hvh, &hvh1, &hvh2);
//    gHvH.play();
//
//------------------------------------BadPlayer vs BadPlayer----------------------------------------------------------
    
//    Board badvbad(3,3);
//    BadPlayer bv1("Bad Player 1");
//    BadPlayer bv2("Bad Player 2");
//    Game bv(badvbad, &bv1, &bv2);
//    bv.play();
    
    //=================================== WORKS CORRECTLY ============================================================
    
//------------------------------------BadPlayer vs SmartPlayer--------------------------------------------------------
    
//    Board badvsmart(3,3);
//    BadPlayer bvs1("Bad Player");
//    SmartPlayer svb1("Smart Player");
//    Game bvs(badvsmart, &bvs1, &svb1);
//    bvs.play();
    
    //=================================== WORKS CORRECTLY ===================================================
    
//------------------------------------SmartPlayer vs SmartPlayer------------------------------------------------------
    
//    Board smartvsmart(3,3);
//    SmartPlayer sP1("Smart player 1");
//    SmartPlayer sP2("Smart player 2");
//    Game svs(smartvsmart, &sP2, &sP1);
//    svs.play();
    
    //=================================== WORKS CORRECTLY ======================================================
    
    cout << "Passed all tests" << endl;
}
