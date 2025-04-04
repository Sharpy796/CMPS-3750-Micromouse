public class Main {
    public static void main(String[] args) {
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


        //MazeBoard maze = new MazeBoard(12, 12);
        MazeBoard premadeMaze = MazeBoard.importMaze(premadeMazeData);

        MazeCell startCell = premadeMaze.getCell(5, 0);
        MazeCell endCell = premadeMaze.getCell(6, 11);
        MazeCell[] path = premadeMaze.findPathBetween(startCell, endCell);

        System.out.println(premadeMaze);
        System.out.println("Shortest path between " + startCell + " and " + endCell + ":");
        for(int i = 0; i < path.length; i++){
            System.out.println(i + ": " + path[i] + " - Distance: " + path[i].getData());
        }
    }
}