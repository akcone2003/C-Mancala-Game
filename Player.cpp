//
//  Player.cpp
//  Project 3
//
//  Created by Aidan Cone on 5/19/23.
//

#include "Player.h"

//===-----------------------------------------------------------------------------------------------------------------------------===//
//
//                                                  Player Implementation
//
//===-----------------------------------------------------------------------------------------------------------------------------===//
Player::Player(std::string name) : mName(name)
{ }

std::string Player::name() const
{
    return mName;
}

bool Player::isInteractive() const
{
    return false;
}

Player::~Player()
{ }

//===------------------------------------------------------------------------------------------------------------------------------===//
//
//                                                  HumanPlayer Implementation
//
//===------------------------------------------------------------------------------------------------------------------------------===//


HumanPlayer::HumanPlayer(std::string name) : Player(name)
{ }

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    int hole;

    while (true) // create infinite loop that only ends when hole is returned
    {
        std::cout << name() << ", enter a hole number: ";
        std::cin >> hole;
        
        if (hole > 0 && hole <= b.holes()) // if valid hole
        {
            if (b.beans(s, hole) != 0) // if beans in the hole
            {
                return hole;
            }
            else
            {
                std::cout << "That hole has no beans. Pick another hole with beans" << std::endl; // no beans in the hole
            }
        }
        else
        {
            std::cout << "That number is invalid. Please pick a valid hole number" << std::endl; // invalid hole
        }
    }
    
    return -1; // no move was possible
}

HumanPlayer::~HumanPlayer()
{
    
}

//===-----------------------------------------------------------------------------------------------------------------------------===//
//
//                                                  BadPlayer Implementation
//
//===-----------------------------------------------------------------------------------------------------------------------------===//

BadPlayer::BadPlayer(std::string name) : Player(name)
{ }

bool BadPlayer::isInteractive() const
{
    return false;
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    for (int hole = 1; hole <= b.holes(); hole++) // loop through holes
    {
        if (b.beans(s, hole) > 0) // if nearest hole has beans, use that hole
        {
            return hole;
        }
    }
    return -1;
}

BadPlayer::~BadPlayer()
{
    
}
//===-----------------------------------------------------------------------------------------------------------------------------===//
//
//                                                  SmartPlayer Implementation
//
//===-----------------------------------------------------------------------------------------------------------------------------===//

SmartPlayer::SmartPlayer(std::string name) : Player(name)
{ }

bool SmartPlayer::isInteractive() const
{
    return false;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const // returns the hole with the highest value after calls of choose move
{
    int depth = 0, bestHole, value;
    chooseMove(b, s, bestHole, value, depth);
    return bestHole;
}

SmartPlayer::~SmartPlayer()
{
    
}

int SmartPlayer::eval(const Board& b) const
{
    int northBeans = (b.beansInPlay(NORTH) + b.beans(NORTH, POT));
    int southBeans = (b.beansInPlay(SOUTH) + b.beans(SOUTH, POT));

    int diff = (southBeans) - (northBeans); // evaluate for SOUTH per spec
    
    if (beansInPlay(b)) // if either side has 0 beans in play
    {                   // determines the best move to make the difference larger for each south and smaller for north
        if (southBeans > northBeans) // if south has more beans
        {
            return INT_MAX; // return max number possible
        }
        else if (southBeans < northBeans) // if north has more beans
        {
            return INT_MIN; // return min number possible 
        }
        else // tie
        {
            return 0;
        }
    }
    
    return diff; // if the game is not over
                 // positive values of diff indicate a favorable position for SOUTH while negative values indicate a favorable position for NORTH
}

void SmartPlayer::chooseMove(const Board& b, Side player, int& bestHole, int& value, int depth) const
{
    if (depth > 6) // Base case 1: depth at 6 allows for maximum time looking the tree while maintaining < 5 seconds
    {
        bestHole = -1;
        value = eval(b); // value is the value of this position
        return;
    }
    
    if (b.beansInPlay(player) == 0) // Base case 2: player has no move
    {
        bestHole = -1;
        value = eval(b); // infinity, -infinity, or 0
        return;
    }
    
    int firstLegalHole = 1; // this ensures smart player always makes a legal move
                       // will at the very least always return the first hole that has beans
    for (int i = 1; i <= b.holes(); i++) // loop thru holes
    {
        Board tempBoard(b); // create copy so we don't change board itself
        Side endSide;
        int endHole;
        int value2, bestHole2, bestHole3; // stores values for the recursive calls for chooseMove
                                          // value2 is the simulated values for the moves
                                          // bestHole2 is the best hole evaluated for the opponent
                                          // bestHole3 is the best hole evaluated for the player's next move
        
        if (b.beans(player, i) == 0) // if hole has no beans ie invalid
        {
            firstLegalHole++; // go to next hole and check if that has beans
            continue;
        }
        
        if (completedMove(player, i, tempBoard, endSide, endHole)) // if a move can be completed
        {                                                    // if evaluates to true
            chooseMove(tempBoard, opponent(player), bestHole2, value2, depth + 1); // sees the possible response to the move made by the player
        }
        else // still player's turn (ie the move did not result in a complete move)
        {
            chooseMove(tempBoard, player, bestHole3, value2, depth); // depth does not increment bc we only need to check opponent's future moves
        } 
        
        tempBoard = b; // unmakes the move
        
        if (i == firstLegalHole) // this is to ensure that the player makes a legal move
        {                   // first if statement in loop body ensures firstHole will = 1 until it is updated again
            value = value2;
            bestHole = i;
        }
        if (player == SOUTH) // if SOUTH, higher values are better
        {
            if (value2 > value) // if value2 is higher value than any value we've seen previously
            {
                bestHole = i;
                value = value2;
            }
        }
        else // if NORTH, lower values are better
        {
            if (value2 < value) // switches because lower values are better for north
            {
                bestHole = i;
                value = value2;
            }
        }
    }
    return;
}

bool SmartPlayer::completedMove(Side s, int hole, Board& b, Side& endSide, int& endHole) const
{
    if (b.sow(s, hole, endSide, endHole)) // if a move exists for chosen hole
    {
        if (endHole == POT) // if on own pot, player takes another turn so it is not a complete move yet.
        {                   // sow always skips opponent's pot so this ensures that it is own player's pot
            return false; // move was not completed
        }
        if (endSide == s) // if ending on own side
        {
            if (b.beans(s, endHole) == 1 && b.beans(opponent(s), endHole) > 0) // if a capture, ends the turn
            {
                b.moveToPot(opponent(s), endHole, s);
                b.moveToPot(s, endHole, s);
            } // need to do capture logic again because sow does not do capture, only move does, and we can't call the move function because we don't have a game object
        }
        return true; // move was completed
    }
    return false; // move was not completed
}

