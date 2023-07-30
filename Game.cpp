//
//  Game.cpp
//  Project 3
//
//  Created by Aidan Cone on 5/19/23.
//

#include "Game.h"

Game::Game(const Board& b, Player* south, Player* north) : mBoard(b), mSouthPlayer(south), mNorthPlayer(north)
{
    currentPlayer = SOUTH; // south begins the game
    
    // create a board, south player and north player
}

void Game::display() const
{
    std::cout << "\t" + mNorthPlayer -> name() << std::endl;
    
    std::cout << "   ";
    for (int i = 1; i <= mBoard.holes(); i++)
    {
        std::cout << " " << std::to_string(mBoard.beans(NORTH, i)) + " ";
        
    }
    std::cout << std::endl;
    
    std::cout << " " + std::to_string(mBoard.beans(NORTH, POT)) + " ";
    for (int i = 1; i <= mBoard.holes(); i++)
    {
        std::cout << "   ";
    }
    
    std::cout << " " + std::to_string(mBoard.beans(SOUTH, POT));
    std::cout << std::endl;
    
    std::cout << "   ";
    for (int i = 1; i <= mBoard.holes(); i++)
    {
        std::cout << " " + std::to_string(mBoard.beans(SOUTH, i)) + " ";
    }
    std::cout << std::endl;
    
    std::cout << "\t" + mSouthPlayer -> name() << std::endl;
    
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    if (mBoard.beansInPlay(NORTH) == 0 && mBoard.beansInPlay(SOUTH) == 0) // no more beans in play
    {
        over = true; // game is over
        
        if (mBoard.beans(NORTH, POT) > mBoard.beans(SOUTH, POT)) // NORTH wins
        {
            hasWinner = true;
            winner = NORTH;
        }
        else if (mBoard.beans(NORTH, POT) < mBoard.beans(SOUTH, POT)) // SOUTH wins
        {
            hasWinner = true;
            winner = SOUTH;
        }
        else // TIE
        {
            hasWinner = false; // no winner
        }
    }
    else
    {
        over = false;
        hasWinner = false;
        return;
    }
    return;
}

bool Game::move(Side s)
{
    Side endSide;
    int endHole;
    
    if (currentPlayer == SOUTH) // currently SOUTH player's turn
    {
        int hole = mSouthPlayer -> chooseMove(mBoard, currentPlayer); // make a turn
        std::cout << std::endl;
        std::cout << mSouthPlayer -> name() + " chooses hole " << hole << std::endl;
        std::cout << std::endl;

        if (mBoard.sow(s, hole, endSide, endHole)) // if able to sow in chosen hole
        {
            if (endHole == POT) // if on own pot (sow always skips opponent pot, so we know this is our own pot)
            {
                display();
                if (mBoard.beansInPlay(SOUTH) > 0) // if game is not over
                {
                    std::cout << mSouthPlayer -> name() + " gets another turn!" << std::endl;
                    return move(currentPlayer); // player gets another turn
                }
            }
            
            if (endSide == currentPlayer && endHole > POT) // ends on own side that is not pot
            {
                if (mBoard.beans(currentPlayer, endHole) == 1 && mBoard.beans(opponent(currentPlayer), endHole) > 0) // capture
                {
                    std::cout << mSouthPlayer -> name() + " captured " + mNorthPlayer -> name() + "'s beans!" << std::endl;
                    std::cout << std::endl;

                    mBoard.moveToPot(opponent(currentPlayer), endHole, currentPlayer); // move opponent's endHole to own pot
                    mBoard.moveToPot(currentPlayer, endHole, currentPlayer); // move last bean in endHole to own pot
                }
            }
        }
        else
        {
            return false;
        }
    }
    else // currently NORTH player's turn
    {
        int hole = mNorthPlayer -> chooseMove(mBoard, currentPlayer); // make a turn
        std::cout << std::endl;
        std::cout << mNorthPlayer -> name() + " chooses hole " << hole << std::endl;
        std::cout << std::endl;
        if (mBoard.sow(s, hole, endSide, endHole)) // if able to sow
        {
            if (endHole == POT) // if on own pot (sow always skips opponent pot)
            {
                display();
                
                if (mBoard.beansInPlay(NORTH) > 0) // game is not over
                {
                    std::cout << mNorthPlayer -> name() + " gets another turn!" << std::endl;
                    return move(currentPlayer); // player gets another turn
                }
            }
            if (endSide == currentPlayer && endHole > POT) // ends on own side that is not pot
            {
                if (mBoard.beans(currentPlayer, endHole) == 1 && mBoard.beans(opponent(currentPlayer), endHole) > 0) // capture
                {
                    std::cout << mNorthPlayer -> name() + " captured " + mSouthPlayer -> name() + "'s beans!" << std::endl;
                    std::cout << std::endl;
                    mBoard.moveToPot(opponent(currentPlayer), endHole, currentPlayer); // move opponent's endHole to own pot
                    mBoard.moveToPot(currentPlayer, endHole, currentPlayer); // move last bean to own pot
                }
            }
        }
        else
        {
            return false;
        }
    }
            
    // current turn cannot be completed because there are no beans on one side
            
    currentPlayer = opponent(currentPlayer); // switch to see if the opposing player has a turn but has no beans which results in sweeping and game over
    
    if (mBoard.beansInPlay(NORTH) == 0) // game is over
    {
        std::cout << std::endl;
        std::cout << "Sweeping remaining beans into " + mSouthPlayer -> name() + "'s pot." << std::endl;
        std::cout << std::endl;
        
        for (int i = 1; i <= mBoard.holes(); i++) // loop thru south holes
        {
            mBoard.moveToPot(SOUTH, i, SOUTH); // sweep all remaining beans on south side into south pot
        }
        display();
        return false; // move cannot be completed
    }
    if (mBoard.beansInPlay(SOUTH) == 0) // game is over
    {
        std::cout << "Sweeping remaining beans into " + mNorthPlayer -> name() + "'s pot." << std::endl;
        for (int i = 1; i <= mBoard.holes(); i++) // loop through north holes
        {
            mBoard.moveToPot(NORTH, i, NORTH); // sweep all remaining beans on north side into north pot
        }
        display();
        return false; // move cannot be completed
    }
    return true; // move was completed
}

void Game::play()
{
    bool gameOver = false, hasWinner = false;
    Side winningSide = SOUTH;
    
    display();
    
    while (!gameOver) // while the game is not over
    {

        move(currentPlayer);
        display();
        
        if (!mSouthPlayer -> isInteractive() && !mNorthPlayer -> isInteractive()) // if both players are computer
        {
            std::cout << "Press ENTER Key to continue game" << std::endl; // need enter key to get a play-by-play
            std::cin.ignore(1000, '\n');
        }
        status(gameOver, hasWinner, winningSide); // update game
    }
    
    if (hasWinner)
    {
        if (winningSide == SOUTH)
        {
            std::cout << mSouthPlayer -> name() + " has won!" << std::endl;
        }
        else
        {
            std::cout << mNorthPlayer -> name() + " has won!" << std::endl;
        }
    }
    else
    {
        std::cout << "The game resulted in a tie." << std::endl;
    }
}

int Game::beans(Side s, int hole) const
{
    if (hole < 0 || hole > mBoard.holes() || s < NORTH || s > SOUTH)
    {
        return -1;
    }
    
    return mBoard.beans(s, hole);
}
