//
//  Board.cpp
//  Project 3
//
//  Created by Aidan Cone on 5/19/23.
//

#include "Board.h"

bool Board::isValid(Side s, int hole) const
{
    return (hole >= 0 && hole <= mNumHoles && s >= NORTH && s <= SOUTH); // checking if s is valid and hole is valid
}


Board::Board(int nHoles, int nInitialBeansPerHole) : mNumHoles(nHoles), mBeansPerHole(nInitialBeansPerHole)
{
    if (nHoles < 0)
    {
        nHoles = 1;
    }
    
    if (nInitialBeansPerHole < 0)
    {
        nInitialBeansPerHole = 0;
    }
    
    mNorthHoles = std::vector<int>(nHoles + 1, nInitialBeansPerHole); // initialize north vector with num holes and initial beans
    mSouthHoles = std::vector<int>(nHoles + 1, nInitialBeansPerHole); // initialize sotuh vector
    
    mTotalBeans = 2 * nHoles * nInitialBeansPerHole; // get the total bean number
    mNorthHoles[POT] = 0;
    mSouthHoles[POT] = 0;
}
//Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.

int Board::holes() const
{
    return mNumHoles;
}

//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
int Board::beans(Side s, int hole) const
{
    if (!isValid(s, hole)) // if invalid
    {
        return -1;
    }
    
    return (s == NORTH) ? mNorthHoles[hole] : mSouthHoles[hole]; // chooses the side and then the number of beans in that hole
}

int Board::beansInPlay(Side s) const
{
    int beansCount = 0;
    
    if (s == NORTH)
    {
        for (int i = 1; i < mNorthHoles.size(); i++)
        {
            beansCount += mNorthHoles[i];
        }
    }
    else
    {
        for (int i = 1; i < mSouthHoles.size(); i++)
        {
            beansCount += mSouthHoles[i];
        }
    }
    return beansCount;
} //Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.

int Board::totalBeans() const
{
    return mTotalBeans;
} //Return the total number of beans in the game, including any in the pots.


bool Board::sow(Side s, int hole, Side& endSide, int& endHole) 
// for south vector ---> if at 6 go to South pot, then go to North's 6
// for north vector ---> if at 1, go to North pot, then go to South's 1
{
    if (!isValid(s, hole) || hole == POT) // is not valid
    {
        return false;
    }
    
    int& currentHoleVector = (s == NORTH) ? mNorthHoles[hole] : mSouthHoles[hole]; // creates reference vector to the hole specified
    int beansToSow = beans(s, hole); // stores number of beans for the specified hole
    
    if (beansToSow == 0) // if hole is empty
    {
        endHole = hole;
        endSide = s;
        return false;
    }
    
    currentHoleVector = 0; // remove beans
    
    int currentHole = hole; // variable for hole
    Side currentSide = s; // variable for side
    
    while (beansToSow > 0) // while there are still beans to sow
    {
        beansToSow--; // decrement one
        
        if (currentSide == NORTH) // if on north, add bean
        {
            if (currentHole == 1) // if start at last hole on north
            {
                currentHole--;
                if (currentHole >= 0) // this check ensures the vector is being referenced within the size of the vector
                {
                    mNorthHoles.at(currentHole) += 1;
                }
            }
            else // start somewhere else
            {
                currentHole--; // go counterclockwise
                if (currentHole >= 0)
                {
                    mNorthHoles.at(currentHole) += 1; // add bean to hole
                }
            }
        }
        else // if on south, add bean
        {
            if (currentHole == mNumHoles) // if on last hole of SOUTH
            {
                currentHole = POT; // switch to pot
                mSouthHoles.at(currentHole) += 1; // add bean to hole
            }
            else // start somewhere else
            {
                currentHole++; // go counterclockwise
                mSouthHoles.at(currentHole) +=1; // add bean to hole
            }
        }
        
        if (currentHole == POT && currentSide != s) // skip if on opponent pot
        {
            continue;
        }

        if (currentHole == POT && s == SOUTH && beansToSow > 0) // if on SOUTH pot and turn has not ended
        {
            currentHole = mNumHoles + 1; // index out of range because it will be decremented later
            currentSide = NORTH; // change to NORTH side
        }
        if (currentHole == POT && s == NORTH && beansToSow > 0) // if on NORTH pot and turn has not ended
        {
            currentHole = 0; // index to the pot because it will be incremented later
            currentSide = SOUTH; // change to SOUTH side
        }
    }
    
    endSide = currentSide; // end side becomes the side where the last bean was placed
    endHole = currentHole; // end hole becomes hole where last bean was placed
    
    return true;
}// If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (!isValid(s, hole)) // if invalid
    {
        return false;
    }
    
    int movedBeans = beans(s,hole); // store current beans in the hole
    
    if (potOwner == NORTH)
    {
        mNorthHoles[POT] += movedBeans; // add the number of beans to the pot
    }
    else
    {
        mSouthHoles[POT] += movedBeans; // add number of beans to the pot
    }
    (s == NORTH) ? mNorthHoles[hole] = 0 : mSouthHoles[hole] = 0; // set the amount of beans in the hole to 0
    
    return true;
} //If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.

bool Board::setBeans(Side s, int hole, int beans)
{
    if (!isValid(s, hole) || beans < 0)
    {
        return false;
    }
    
    int& currentHole = (s == NORTH) ? mNorthHoles[hole] : mSouthHoles[hole]; // create reference to that hole
    mTotalBeans -= currentHole; // subtract original amount of beans from total
    currentHole = beans; // add beans to the current Hole
    mTotalBeans += currentHole; // add number of beans to the total beans count 
    
    return true; 
} //If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
