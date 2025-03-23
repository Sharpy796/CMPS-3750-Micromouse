public class MazeCell {
    private MazeCell up;
    private MazeCell down;
    private MazeCell left;
    private MazeCell right;

    private final int x;
    private final int y;
    private int data; // For holding distance values from the flood fill algorithm

    public MazeCell(int x, int y){
        up = null;
        down = null;
        left = null;
        right = null;

        this.x = x;
        this.y = y;
        data = -1;
    }

    // ----------------------------------------------------------
    // SETTERS FOR ADJACENT CONNECTED CELLS
    // ----------------------------------------------------------
    public void setCellAbove(MazeCell other){
        up = other;
        if(other.getCellBelow() == null){ // Cell connections should be a two-way relationship
            other.setCellBelow(this);
        }
    }

    public void setCellBelow(MazeCell other){
        down = other;
        if(other.getCellAbove() == null){ // Cell connections should be a two-way relationship
            other.setCellAbove(this);
        }
    }

    public void setCellLeft(MazeCell other){
        left = other;
        if(other.getCellRight() == null){ // Cell connections should be a two-way relationship
            other.setCellRight(this);
        }
    }

    public void setCellRight(MazeCell other){
        right = other;
        if(other.getCellLeft() == null){ // Cell connections should be a two-way relationship
            other.setCellLeft(this);
        }
    }

    // ----------------------------------------------------------
    // GETTERS FOR ADJACENT CONNECTED CELLS
    // ----------------------------------------------------------
    public MazeCell getCellAbove(){
        return up;
    }

    public MazeCell getCellBelow(){
        return down;
    }

    public MazeCell getCellLeft(){
        return left;
    }

    public MazeCell getCellRight(){
        return right;
    }

    // ----------------------------------------------------------
    // DESTROYING THE CONNECTION BETWEEN TWO ADJACENT CELLS
    // ----------------------------------------------------------
    public void addWallAbove(){
        if(up != null){
            MazeCell other = up;
            other.down = null;
            up = null;
        }
    }

    public void addWallBelow(){
        if(down != null){
            MazeCell other = down;
            other.up = null;
            down = null;
        }
    }

    public void addWallLeft(){
        if(left != null){
            MazeCell other = left;
            other.right = null;
            left = null;
        }
    }

    public void addWallRight(){
        if(right != null){
            MazeCell other = right;
            other.left = null;
            right = null;
        }
    }

    // ----------------------------------------------------------
    // MAZE CELL DATA
    // ----------------------------------------------------------
    public void setData(int newData){
        data = newData;
    }

    public int getData(){
        return data;
    }

    @Override
    public String toString() {
        return ("MazeCell at (" + x + ", " + y + ")");
    }
}
