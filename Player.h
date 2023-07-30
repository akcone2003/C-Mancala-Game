//
//  Player.hpp
//  Project 3
//
//  Created by Aidan Cone on 5/19/23.
//

#ifndef Player_h
#define Player_h

#include "Side.h"
#include "Board.h"
#include <iostream>
#include <string>
#include <climits>

class Player
{
public:
    Player(std::string name);
        //Create a Player with the indicated name.
    std::string name() const;
    virtual bool isInteractive() const;
        //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const = 0;
        //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player();
    
private:
    std::string mName;
};
//Since this class is designed as a base class, it should have a virtual destructor.

class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name);
        //Create a Player with the indicated name.
    virtual bool isInteractive() const;
        //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const;
        //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~HumanPlayer();
};

class BadPlayer : public Player
{
public:
    BadPlayer(std::string name);
        //Create a Player with the indicated name.
    virtual bool isInteractive() const;
        //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const;
        //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~BadPlayer();
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name);
        //Create a Player with the indicated name.
    virtual bool isInteractive() const;
        //Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const;
        //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~SmartPlayer();
    
private:
    int eval(const Board& b) const; // evaluates the current position of the hole on the board based on heuristics
    void chooseMove(const Board& b, Side player, int& hole, int& value, int depth) const; // minimax algorithm that chooses the move for the smart player
    bool completedMove(Side s, int hole, Board& b, Side& endSide, int& endHole) const; // returns true if a move is complted, false if not
    inline bool beansInPlay(const Board& b) const
    {
        return (b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0);
    } // returns true if either side has 0 beans in play, false otherwise
};
#endif /* Player_hpp */
