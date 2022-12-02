//Given the size and contents of the maze, figure out the fewest number of moves Sastry needs to 
//make to get out of the maze.

public class maze {
	
	
	public static int ROW, COL;
	public static int startX, startY;
	final public static int[] DX = {-1,0,0,1};
	final public static int[] DY = {0,-1,1,0};
	public static char[][] maze;
	static boolean visited[][];
	public static Queue<Integer> qRow = new LinkedList<Integer>();
	public static Queue<Integer> qColumn = new LinkedList<Integer>();
	public static Map<String, ArrayList<coordinate>> map = new HashMap<String, ArrayList<coordinate>>();
	public static Set<String> set = new HashSet<>();
	public static int nodesNext,nodesLeft,count;
	
	
	// finds starting point and groups all teleportation  squares
	public static void findStart() {
		for(int i = 0; i < ROW; i++) {
			for(int j=0; j < COL; j++) {
				if(Character.isLetter(maze[i][j])) {
					String tmp = Character.toString(maze[i][j]); 
					if(map.containsKey(tmp)) {
						map.get(tmp).add(new coordinate(i,j));
					}
					else {
						map.put(tmp, new ArrayList<coordinate>());
						map.get(tmp).add(new coordinate(i,j));
					}
				}
				if(maze[i][j] == '*') {
					startX = i;
					startY = j;
				}
			}
		}
	}
	
	// returns least amount of moves required to escape or -1 if escape is not possible 
	public static int solve(int row, int col) {
		boolean done = false;
		qRow.add(row);
		qColumn.add(col);
		visited = new boolean[ROW][COL];
		visited[row][col] = true;
		while(!qRow.isEmpty()) {
			row = qRow.remove();
			col = qColumn.remove();
			
			if (maze[row][col] == '$') {
                done = true;
                break;
            }
			
			findAdjacents(row,col);
			nodesLeft--;
			if(nodesLeft == 0) {
				nodesLeft = nodesNext;
				nodesNext = 0;
				count++;
			}
			
		}
		if(done) return count;
		
		return -1;
	}
	
	// finds the coordinates that are adjacent to the current location
	public static void findAdjacents(int row, int col) {
		// finds adjacent coordinates in the DX DY direction
		for (int dir=0; dir<DX.length; dir++) {
			int nX = row + DX[dir];
			int nY = col + DY[dir]; 
			// Can't go out of bounds 
			if (!inbounds(nX,nY)) continue;
			qRow.add(nX);
			qColumn.add(nY);
			visited[nX][nY] = true;
			nodesNext++;
		}
		if(Character.isLetter(maze[row][col]) && set.contains(Character.toString(maze[row][col])) == false) {
			// loop through each coordinate in the Letters arrayist and add it
			for(int i = 0; i < map.get(Character.toString(maze[row][col])).size(); i++) {
				qRow.add( map.get(Character.toString(maze[row][col])).get(i).getX());
				qColumn.add( map.get(Character.toString(maze[row][col])).get(i).getY());
				visited[ map.get(Character.toString(maze[row][col])).get(i).getX()][map.get(Character.toString(maze[row][col])).get(i).getY()] = true;
				nodesNext++;
			}
			// mark if letter has been processed 
			set.add(Character.toString(maze[row][col]));
		}
		
	}
	
	// find if coordinate is out of bounds or not a possible move
	public static boolean inbounds(int nX, int nY) {
		if( nX < 0 || nY < 0) {
			return false;
		}
		if(nX >= ROW || nY >= COL) {
			return false;
		}
		if(maze[nX][nY] == '!' || visited[nX][nY] == true) {
			return false;
		}
		return true;
	}
	  
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		int answer = 0;
		nodesNext=0;
		nodesLeft = 1;
		count=0;
		// get values for row and columns
		String row = in.next();
		String col = in.next();
		ROW = Integer.parseInt(row);
		COL = Integer.parseInt(col);
		
		// Initialize maze board
		maze = new char[ROW][COL];
		for (int i=0; i<ROW; i++){
			String line = in.next();
			for(int j=0;j<COL;j++) {
				maze[i][j] = line.charAt(j);
			}
		}
		
		// find the starting position
		findStart();
		
		// get the answer
		answer = solve(startX,startY);
		System.out.print(answer);
	}
}

class coordinate{
	private int x;
	private int y;
	public coordinate(int x, int y) {
		this.x = x;
		this.y =y;
	}
	public int getX() {
		return x;
	}
	public int getY() {
		return y;
	}
	public void setX(int newx) {
		this.x = newx;
	}
	public void setY(int newy) {
		this.y = newy;
	}
}

 
