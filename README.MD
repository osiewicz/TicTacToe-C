# TicTacToe
## Current version: 1.7.0
#### A simple implementation of immortal game, where two players, X and O, take turns marking the spaces in a 3×3 grid. The player who succeeds in placing three of their marks in a horizontal, vertical, or diagonal row wins the game.

### Fore(after)word:
Tic Tac Toe might seem redundant to implement. However, there is some theory to it that makes it an interesting (yet simple) game - this project uses  
MinMax with AlphaBeta pruning to make sure that AI stays top notch. Unfortunately, on my machine (Intel i5-4670k) it takes way too long for AI to play on 6x6 board.  
That is my next focus - making AI scalable for *any* (within realms of possibility) board size. 
If (for whatever reason) you are interested in history of this project, old README files have some of that.

### Installation  
To install Tic Tac Toe, just run `make` in `TicTacToe/src` . Then, all you need to do is to type "./Tic-Tac-Toe" into your terminal. :) 

### Contributing  
If you want to contribute to the project by creating translation file (or improving the code), feel free to do so.   
All you need to do is follow the convention of .lang files - each line has its corresponding index in the string table, so it should be pretty straight forward to create your own translation.   

### Starting parameters
**TL;DR:**  

**-l / --language** -set language to the corresponding ._LANGUAGE_.lang . (default: en-gb)  

**--p1_name** -set player 1 name. (default: Player 1)  

**--p2_name** -see p1_name. (default: Player 2)  

**--AI_name** -see p1_name. (default: AI)  

**-s / --b_size** - set board size to corresponding integer (default: 3)  

**--p1_sign** -set player 1 sign. (default: X)  

**--p2_sign** -see p1_sign. (default: O)  

**--ff_sign** -Stands for 'free field sign'. see p1_sign. (default: #)  

**--ai_vs_ai** -Let AI play versus itself when AI is chosen in menu. (default: 0)


