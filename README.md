# C++-Mancala-Game
C++ Mancala game with an AI player that picks the best possible move every turn

The Basics of the Game

Mancala is a two-person strategy game traditionally played with beans and a board. Each of the two players, North and South, has six holes and a pot on their side. Each hole starts off with a pre-compile specified amount of beans and takes a turn. A move will begin with the player picking up the beans in the hole they have chosen and, going counterclockwise, puts one bean in each subsequent hole and their own pot (but skips over the opponent's pot). This happens until all of the beans that were picked up have been put into a hole, otherwise known as being sown. Depending on where the last bean was placed, one of three things can happen:
1)	Capture - If the last bean was placed in one of the player’s own holes that was previously empty before the turn (i.e. it has one bean in it after the turn) and if the hole directly across on the opponent’s side is not empty, then the last bean on the player’s side and all the beans in the hole directly opposite are put into the player’s own pot.  
2)	If the last bean was placed in the player’s own pot, the player takes another turn.
3)	The player’s turn ends.
When a player tries to take their turn, but cannot do so because their side has no beans at all, any beans remaining in the opponent’s side are put into the opponent’s pot and the game is over. The winner is whoever has the most amount of beans in their pot. 

The AI Breakdown

The AI player utilizes a minmax algorithm that searches the game tree for the best play available in any given turn. The minimax algorithm will take the best move according to an evaluation function that computes the difference between the number of beans in each player’s pot. It will preference moves that result in a greater difference between the pots, which has different values based on which side the player is. For South, it will preference higher values (since the function is modeled for South). However, for North, it will preference lower values, since these are bad for South which makes them good for North. It is limited by a depth variable to keep within a certain time limit, but it can be changed to search many more or many less levels of the game tree. 
