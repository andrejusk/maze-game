# QuickSortC
Piece of university coursework that implements maze generation in C++, allows users to navigate and complete the maze and features dynamic lighting.

## Usage
Compile and run in cmd, powershell, terminal or double-click.

## Algorithm
A [depth-first search maze generation algorithm](https://en.wikipedia.org/wiki/Maze_generation_algorithm#Depth-first_search) is used to create unique maps for every playthrough. Below is the algorithm used.
```ps
GenerateMaze()
	CurrentCell = InitialCell
	While Map.HasUnvisitedCells do 
		If CurrentCell.hasUnvisitedNeighbours then
            TempCell = CurrentCell
			CurrentCell = CurrentCell.RandomNeighbour()
			RemoveWall(CurrentCell, TempCell)
			Stack.push(TempCell)
		Else
			CurrentCell = Stack.pop()
		End if		
	End loop
```
Below is a sample of a map that is generated using this algorithm.

[![Sample Map](https://i.imgur.com/xuInZGx.png)](https://i.imgur.com/xuInZGx.png)

## Interface
The interface was designed to be simple and console-based. Dynamic lighting was implemented to make exploring the maze a more interesting experience. This was done by projecting player "vision" in every direction until a wall was hit. Score is added to the player based on the number of tiles explored (i.e. "seen").

[![CLI](https://i.imgur.com/OoD2uXt.png)](https://i.imgur.com/OoD2uXt.png)