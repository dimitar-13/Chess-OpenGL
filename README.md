# Chess-OpenGL

## About the game
A simple chess game using OpenGL. The game is 2 players only for now and it supports the
following features:
 - **Pawn promotion.**
 - **Check and checkmate logic.**
 - **Movement validation.**
 - **Valid movement visualization**

Features to add:
 - **King and Rook Position Switching (Castling)**
 - **En Passant**:Allows a pawn that has moved two squares forward from its starting position to be captured by an opposing pawn that is adjacent to it, as if it had only moved one square.
 - **Stalemate logic.**
 - **Switching chess piece skins.**

## Some screenshots
![Main menu screen](/git_images/git_image_main_menu.png?raw=true)
![Project wireframe rendering image](/git_images/git_image_chessboard.png?raw=true)
![Pawn promotion](/git_images/git_image_pawn_promotion.png?raw=true)
![Possible movement](/git_images/git_image_piece_move.png?raw=true)
![Game over screen](/git_images/git_image_game_over_screen.png?raw=true)

## How to build ?
First clone the project using the following command:
```git
git clone https://github.com/dimitar-13/Chess-OpenGL.git --recursive
```
The command will recursively clone the project and all the dependencies the project have(GLFW ,assimp etc.)

To build the project you will need to have Cmake(I don't recement the GUI version because the cmd input is only 1 command).
Then you open cmd in the cloned project folder (where the `src` folder is) and use this command:
```
cmake -S src/ -B build/
```
This will create a `build` folder where the Visual Studio project will be generated.


## Things I learned
The main goal of the project was to learn how games using only OpenGL are made. I've learned a lot
more then that.

- I learned how to make the code more reusable using composition to implement the 
`ChessPiece` movement logic.
- I learned something about making UI system. While the one I did could merely be called UI system 
	I'm quite happy how it turned out in the end.
- I learned how to use multithreading to implement the pawn promotion logic without blocking the 
	main thread.
- I leaned to optimize the allocation using a MemoryPool to pre-allocate a big chunk of memory.


## About the game(technical level)

### Chess game logic

The chess game logic is created in a such way to not depend on a user interface. The game logic can used 
in headless mode(using only the console). You can also make it so that the game is run in 3D. 

There are 3 main parts of the chess game logic:
1. **Chess piece.**
2. **Chess player.**
3. **Chess game.**

### Chess piece
Every chess piece in the project has what is known as a `MovementRule`. 
A movement rule defines a specific movement behavior in chess, such as the 
diagonal movement of a bishop. Each chess piece has a vector of these rules.

`MovementRule` is an interface that contains one pure virtual function, `CanMove`. 
This function takes a start and an end position as input and returns `true` if the
movement is valid according to the specified rule.

In addition to `MovementRule`, which is isolated from the board and does not account for checks
such as out-of-bounds movement or pieces blocking the path, there is `BoardSpecificMovementRule`. 

`BoardSpecificMovementRule` is another interface, but it extends the functionality by 
incorporating board state information. 
Its `CanMove` function takes two positions and a `ChessBoardBitMask` as parameters. 
The `ChessBoardBitMask` provides the following details for each board position:
- **Occupancy**: Whether a position is occupied.
- **Team**: Which team occupies the position.
- **Immortality**: Whether the occupying piece is immortal (e.g., a King that cannot be captured).

### Chess player
The `ChessPlayer` class functions as a named array. 
By this, it means that the class holds a `std::vector` of `ChessPiece` objects and provides
functionality for interacting with these pieces. Additionally, the `ChessPlayer` class keeps
track of the currently selected piece.

### Chess game
The `ChessGame` class is the highest-level class in the project. 
It manages two players and includes functionalities for implementing check and checkmate logic.

> [!NOTE]
> This is explained in detail within the project, but the main idea is that 
> `ChessPiece` objects are not aware of other chess pieces or the board state.
> Instead, higher-level classes like `ChessPlayer` and `ChessGame` are responsible 
> for managing interactions between pieces and implementing game logic using the `ChessPiece` class.


## Project UI logic

The project uses a custom UI system designed for simplicity and flexibility. 
It allows the easy creation and management of UI elements, with a structure built around a hierarchy of panels and widgets.

### Hierarchical Structure

- **Panels and Widgets**: Every UI widget is assigned a parent UI element. By default, new widgets are assigned the `RootPanel` as their parent, which represents the window.	
This hierarchy allows for the grouping and management of UI elements under different panels.
- **Grouping**: Additional panels can be created to group multiple UI elements, providing a structured and organized layout.

### Positioning System

The positioning of UI elements is achieved using a combination of:

1. **Pivot Point**: An imaginary reference point within the parent panel. The pivot point can be one of the following:
   - **Top Left**
   - **Top Right**
   - **Bottom Left**
   - **Bottom Right**
   - **Middle**

2. **Offset**: Specifies the distance from the selected pivot point. 
This offset allows for flexible positioning relative to the pivot, 
making the UI semi-responsive while keeping the design simple.

By combining pivot points and offsets, our UI system maintains a balance
between responsiveness and ease of use.

## Project drawing logic
The project uses a batch rendering technique to draw everything from 2D sprites to UI to text.


## Art credits
- **Chess pieces**
	- Author:`drummyfish`
	- **[Resource link](https://opengameart.org/content/chess-pieces-and-a-board)**
- **UI art**
	- Author:`tiopalada`
	- **[Resource link](https://opengameart.org/content/tiny-rpg-dragon-regalia-gui)**
