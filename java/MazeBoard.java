import java.util.LinkedList;
import java.util.Queue;

/* ------------------------------------------------------------------------------------------------------
                                        MICRO MOUSE MAZE IN MEMORY
                                            By Wren Caillouet
   ------------------------------------------------------------------------------------------------------
    The MazeBoard class is built around a 2D array of MazeCell objects. The MazeCell class is simple,
    inspired by the logic of a doubly linked list; a MazeCell object only consists of four pointers to
    other MazeCell objects, one for each adjacent connected cell in the maze (above, below, left, and
    right). If any of these pointers are null, that means no connection exists in that direction, which
    means either the cell is at the edge of the maze, or it is separated from the adjacent cell by a wall.

    The MazeBoard class works on the assumption that a wall does not exist in the maze until proven
    otherwise. Thus, a newly made MazeBoard object will contain an empty maze. Only two pieces of info
    are needed to add a wall to a maze:
        1) The cell the wall is adjacent to, and
        2) Which side of the cell a wall occupies.
    This will allow our MicroMouse to learn any maze layout as it traverses, commiting walls it sees to
    memory based off its position and the direction it's facing when a new wall is found.

    Further explanations will be added throughout the code.
   ------------------------------------------------------------------------------------------------------ */


public class MazeBoard {
    private final int width, height;
    private final MazeCell[][] maze; // This is the backbone of the MazeBoard object.

    // These two variables are for handling printing the maze to the output log, for easy debugging.
    private final int displayWidth, displayHeight;
    private final String[][] displayGrid;

    public MazeBoard(int width, int height){
        this.width = width;
        this.height = height;
        maze = new MazeCell[width][height];

        // ------------- Populating the maze 2D array with MazeCell objects -------------
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                maze[i][j] = new MazeCell(i, j);
            }
        }

        // ------------- Declare relationships between adjacent MazeCell objects -------------
        // setCellBelow() and setCellRight() automatically establish the inverse connections
        // so they need not be handled in this loop.
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                MazeCell curCell = maze[i][j];
                if(i + 1 < height){ // Ensure we do not search out of bounds
                    curCell.setCellRight(maze[i+1][j]);
                }

                if(j + 1 < width) {
                    curCell.setCellBelow(maze[i][j + 1]);
                }
            }
        }

        // ------------- Initialize maze printing functionalities -------------
        // When printing, any single maze cell is represented by a 2*2 subsection in the
        // display, showing itself and the walls (or lack thereof) to below and to its right,
        // taking on one of these 4 states.
        //      [  ,  ]  or [  ,##]  or [  ,  ]  or [  ,##]
        //      [  ,..]     [  ,##]     [##,##]     [##,##]
        // When put together like tiles, the whole maze can be assembled and then printed.

        displayWidth = width * 2 + 1; // +1 to accommodate the left and top borders tho
        displayHeight = height * 2 + 1;
        displayGrid = new String[displayWidth][displayHeight];
        initGrid(); // Populates the display array to ensure no null values exist in its
    }

    public MazeCell getCell(int x, int y){
        return maze[x][y];
    }

    // ----------------------------------------------------------
    // FLOOD FILL
    // ----------------------------------------------------------
    // This function takes a starting cell and fills all other cells with their distance from that starting cell.
    public void floodFill(MazeCell startingCell){
        // Reset all cells to have no distance value, in case flood fill has been performed before
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                maze[i][j].setData(-1);
            }
        }

        // Queue for holding the cells in the maze that still need to be processed
        Queue<MazeCell> cellQueue = new LinkedList<>();
        startingCell.setData(0); // The distance a cell is from itself is 0 (lol)
        cellQueue.offer(startingCell); // Tip the first domino

        MazeCell curCell;
        int curDepth;
        MazeCell[] connections;

        while(!cellQueue.isEmpty()){
            curCell = cellQueue.poll(); // Get the next cell to be processed
            curDepth = curCell.getData(); // Fetch its distance from the starting cell

            // Fetch the connected cells to our current cell
            connections = new MazeCell[]{
                    curCell.getCellAbove(),
                    curCell.getCellRight(),
                    curCell.getCellBelow(),
                    curCell.getCellLeft()
            };

            for(MazeCell connectedCell : connections){
                if(connectedCell != null){ // If a connected cell exists
                    // and that cell hasn't been processed OR was processed as part of a *longer* path
                    if(connectedCell.getData() < 0 || connectedCell.getData() > curDepth + 1){
                        connectedCell.setData(curDepth + 1); // Set its distance value - 1 farther away from current cell
                        cellQueue.offer(connectedCell); // Add it to the queue to repeat the cycle once more
                    }
                }
            }
        }
    }

    // ----------------------------------------------------------
    // FIND THE SHORTEST PATH USING FLOOD FILL
    // ----------------------------------------------------------
    // This function returns an array consisting of the MazeCells which trace out the shortest path between the
    // specified starting cell and ending cell, using the flood fill algorithm.
    public MazeCell[] findPathBetween(int x1, int y1, int x2, int y2){
        MazeCell startCell = getCell(x1, y1);
        MazeCell endCell = getCell(x2, y2);

        return findPathBetween(startCell, endCell);
    }

    public MazeCell[] findPathBetween(MazeCell startCell, MazeCell endCell){
        floodFill(endCell);
        // Flood filling the maze starting at the desired end cell of our path means that the only 0 distance value
        // will be placed at our destination. This means that our starting cell will have a value greater than zero,
        // and finding the next step in our path is easy as finding which adjacent cell has a lower distance value -
        // putting us closer to the destination

        // Queue for holding the cells we find along our path
        Queue<MazeCell> path = new LinkedList<>();
        MazeCell[] connections;

        path.offer(startCell);
        MazeCell curCell = startCell; // Tip the first domino

        // While the distance is greater than 0 (meaning we haven't found our destination yet)
        while(curCell.getData() > 0){
            // Fetch the connected cells to our current cell
            connections = new MazeCell[]{
                    curCell.getCellAbove(),
                    curCell.getCellRight(),
                    curCell.getCellBelow(),
                    curCell.getCellLeft()
            };

            for(MazeCell connectedCell : connections){
                if(connectedCell != null){ // If the connected cell exists
                    if (connectedCell.getData() < curCell.getData()) { // And its distance from end is lower
                        path.offer(connectedCell); // It must be along our path
                        curCell = connectedCell; // Make it our next step and repeat the cycle
                        break;
                    }
                }
            }
        }

        MazeCell[] pathAsArray = new MazeCell[path.size()];
        path.toArray(pathAsArray);
        return pathAsArray;
    }


    // ----------------------------------------------------------
    // IMPORTING A PRE-EXISTING MAZE
    // ----------------------------------------------------------
    // Honestly, this entire section is just for making debugging and testing easy.
    // It can (and likely should) be removed from the final MicroMouse.

    /*   -------------- IMPORT KEY --------------------
                0 - NO WALLS
                1 - WALL RIGHT
                2 - WALL BELOW
                3 - WALLS RIGHT AND BELOW

        -------------- EXAMPLE  --------------------
        int[][] premadeMazeData = {
            {0, 1, 0, 1, 2, 0, 1, 0, 3, 0, 1, 1},
            {1, 3, 1, 2, 2, 3, 1, 2, 2, 3, 0, 1},
            {2, 2, 3, 0, 2, 1, 1, 0, 1, 0, 3, 1},
            {2, 1, 0, 3, 1, 2, 3, 1, 1, 2, 1, 1},
            {0, 3, 1, 0, 3, 2, 0, 3, 0, 3, 1, 1},
            {0, 2, 1, 1, 2, 2, 2, 3, 1, 0, 1, 3},
            {0, 1, 2, 2, 2, 2, 2, 0, 2, 3, 2, 1},
            {3, 1, 0, 1, 0, 2, 1, 3, 0, 2, 1, 1},
            {0, 3, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1},
            {0, 1, 1, 2, 1, 0, 3, 1, 1, 2, 3, 1},
            {1, 2, 2, 1, 3, 1, 0, 3, 0, 0, 2, 3},
            {2, 2, 3, 2, 2, 3, 3, 2, 3, 2, 2, 3}
        };
        MazeBoard premadeMaze = MazeBoard.importMaze(premadeMazeData);
    */

    public static MazeBoard importMaze(int[][] data){
        int width = data.length;
        int height = data[0].length;
        MazeBoard newMaze = new MazeBoard(width, height);

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                int value = data[j][i];

                if(value != 0){
                    MazeCell curCell = newMaze.getCell(i, j);

                    if(value == 1){
                        curCell.addWallRight();
                    } else if(value == 2){
                        curCell.addWallBelow();
                    } else if(value == 3){
                        curCell.addWallRight();
                        curCell.addWallBelow();
                    } else {
                        System.out.println("Invalid cell value detected");
                    }
                }
            }
        }

        return newMaze;
    }

    // ----------------------------------------------------------
    // MAZE DISPLAY FUNCTIONS
    // ----------------------------------------------------------

    // Putting these at the top for easy customization :3
    private final String wall = "||";
    private final String empty = "  ";

    // Populates the display array to ensure no null values exist in it
    public void initGrid(){
        for(int i = 0; i < displayWidth; i++) {
            for (int j = 0; j < displayHeight; j++) {
                if(i == 0 || j == 0 ){ // Add the wall along the top and left border
                    displayGrid[i][j] = wall;
                } else {
                    displayGrid[i][j] = empty;
                }
                // The walls for the right and bottom borders are done automatically
                // when the display is updated for the cells along them (they will
                // have null values for their down/right pointers)
            }
        }

        updateGrid();
    }

    // Updates the state of the maze's display representation, showing any newly added walls
    public void updateGrid(){
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                updateCell(i, j);
            }
        }
    }

    // Handles updating a single cell's 2*2 subsection in the final display.
    // NOTE: This functionality is coded pretty inefficiently. This is done intentionally to keep the MazeCell class
    // as simple as possible, as the ability to print the maze is only needed as a debugging tool.
    public void updateCell(int x, int y){
        MazeCell curCell = maze[x][y];
        int dx = x*2 + 1; // Because the display representation is not 1:1, we need to find the
        int dy = y*2 + 1; // top-right corner of our cell's position in the display

        displayGrid[dx][dy] = String.format("%2s", curCell.getData());
        displayGrid[dx + 1][dy + 1] = wall;

        if(curCell.getCellBelow() == null){
            displayGrid[dx][dy + 1] = wall;
            displayGrid[dx + 1][dy + 1] = wall;
        }

        if(curCell.getCellRight() == null){
            displayGrid[dx + 1][dy] = wall;
            displayGrid[dx + 1][dy + 1] = wall;
        }
    }

    // Prints the maze by stringing the rows of the 2D String array representation into one long
    // continuous string with line breaks
    public String toString() {
        updateGrid(); // Make sure it's the most recent version!
        StringBuilder output = new StringBuilder();
        for (int y = 0; y < displayWidth; y++) {
            for (int x = 0; x < displayHeight; x++) {
                output.append(displayGrid[x][y]);
            }
            output.append("\n");
        }
        return output.toString();
    }
}
