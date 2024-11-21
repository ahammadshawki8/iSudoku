# iGraphics Project for CSE102
![poster](resources/readme_show.png)

### Project Name: iSudoku

### Project Description:
Sudoku is a logic-based number-placement puzzle. The goal is to fill a 9x9 grid with numbers so that each row, each column, and each of the nine 3x3 subgrids (also called "boxes") contains all of the digits from 1 to 9 without repetition.
Here’s a breakdown of the rules:
- Rows: Each row must contain every number from 1 to 9, without repetition.
- Columns: Each column must contain every number from 1 to 9, without repetition.
- 3x3 subgrids: Each of the nine 3x3 subgrids must contain every number from 1 to 9, without repetition.

The puzzle typically starts with some numbers already filled in, and the player needs to deduce the missing ones.

### Game Design:
This iGraphics-based Sudoku game will feature four distinct screens to guide the player through the experience:
- Home Screen: The introductory screen where the player begins their journey, offering options to start a new game or access game settings.
- Level/ Difficulty Selection Screen: A screen that allows the player to choose their desired level of difficulty, ranging from easy to hard, to match their skill and challenge preference.
- Gameplay Window: The main game interface where the player can interact with the Sudoku grid, fill in numbers, solve puzzles, and use hints or tools as needed.
- End Screen: The concluding screen that appears once the game is completed displays the results, including whether the puzzle was solved correctly, along with options to restart or exit the game.
Leaderboards Screen: This screen is displayed after completing a level, offering players an opportunity to register their names and the time taken to solve the puzzle. It showcases the top six fastest solvers, fostering a sense of competition and achievement. Players can compare their performance against others and strive to improve, adding a competitive and rewarding aspect to the game.
- How to Play Screen: This window explains Sudoku rules and guidelines for beginners, helping anyone understand and play the game confidently.

These six windows ensure a seamless and engaging experience for the player, from start to finish.

### Detailed Overview of Gameplay Window:
The Gameplay Window is the central hub where players interact with the Sudoku puzzle. Here's a breakdown of its features:
- Color-Coded Sudoku Grid: The main display consists of a 9x9 Sudoku grid, where each cell is either pre-filled with an initial number or left empty for the player to complete. The grid will be color-coded to visually distinguish between filled and empty cells, making it easier to navigate and solve.
- Cell Navigation: Players can freely navigate through the grid, selecting any cell they wish to focus on. The interface allows smooth movement through both filled and empty cells, providing a seamless experience as they work through the puzzle.
- Cell Editing: Players can only input or modify numbers in the empty cells. This ensures that the player’s task is to logically fill in the missing numbers to complete the puzzle while adhering to Sudoku rules.
- Timer: A built-in timer tracks the player's progress, counting up from zero each time a new level or puzzle begins. This feature adds an element of challenge, encouraging players to complete the puzzle in the shortest time possible.
- Submit Button: Once the player feels confident in their solution, they can press the Submit button to check their work. If the solution is correct, the game will end, and the player can proceed to the next level or exit.
- Autosolve Button: For players who are stuck or simply want to see the solution, an Autosolve button is available. When clicked, this button uses the backtracking algorithm to automatically solve the puzzle, providing a quick and accurate resolution
 
### Backtracking Algorithm:
The backtracking algorithm is a depth-first search technique used to solve Sudoku by trying out different number placements for each empty cell. It works by filling in the grid one cell at a time and checking if the current number violates Sudoku’s rules (no duplicates in rows, columns, or subgrids). If a conflict is found, the algorithm backtracks by undoing the last placement and trying a different number. This process continues until the puzzle is solved or all options are exhausted. It's an efficient way to explore possible solutions systematically.

