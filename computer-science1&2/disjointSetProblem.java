// Given a network of n computers, a set of the pairs of computers that are initially connected, and a 
//sequence  of  steps  where  connections  are  destroyed,  one  by  one,  calculate  the  connectivity  (as 
//defined in the problem specification above) after each connection is destroyed. 


import java.util.*;

public class Destroy {

	public static int[] nodes;
	public int[] parent;
	static ArrayList<Long> end = new ArrayList<Long>();
	public static long size;
	public static long a,b;

	// Create disjoint set
	public Destroy(int n) {
		parent = new int[n];
		for (int i=0; i<n; i++)
			parent[i] = i;
	}

	public int find(int v) {
		// Root of tree found
		if (parent[v] == v) return v;

		// Find root
		int res = find(parent[v]);

		// Attach to root of tree
		parent[v] = res;
		return res;
	}
	
	// Unions pairs, returns true if roots are not equal
	public boolean union(ArrayList<Integer> destroyPairs, ArrayList<IntPair> setPairs,int i) {
		
		int id1 = setPairs.get(i).one;
		int id2 = setPairs.get(i).two;
		
		// Find respective roots.
		int rootv1 = find(id1);
		int rootv2 = find(id2);

		// No union done, v1, v2 already together.
		if (rootv1 == rootv2) return false;
		
		// Decide if root1 or root2 should become parent, update a and b
		if(nodes[rootv1] >= nodes[rootv2]) {
			a= nodes[rootv1];
			b= nodes[rootv2];
			nodes[rootv1] += nodes[rootv2];
			nodes[rootv2]=1;
			parent[rootv2] = rootv1;
			
		}
		else { 
			a= nodes[rootv1];
			b= nodes[rootv2];
			nodes[rootv2] += nodes[rootv1];
			parent[rootv1] = rootv2;
			nodes[rootv1] = 1;
		}
		return true;
	}
	
	// Find the connectivity of the current sets
	public static void calculate(int num) {
		long answer;
		long nodeS = a+b;
		nodeS = nodeS * nodeS;
		answer = size - (a*a) - (b*b) + nodeS;
		size = answer;
		end.add(answer);
	}
	
	// Get the initial Connectivity of the sets
	public static void initializeSize(int numNet) {
		size=0;
		int done =0;
		int[] sorted = nodes.clone();
		Arrays.sort(sorted);
		for(int i=numNet-1; i>0;i--) {
			done+= sorted[i];
			if(done==numNet) {
				end.add(size);
				return;
			}
			size = size + ((sorted[i])*(sorted[i]));
		}
		end.add(size);
	}

	
	public static void main(String[] args) {

		Scanner in = new Scanner(System.in);
	
		int numNet; // number of computers in enemy networks 
		int numConnect; // number of connections between pairs of computers in enemy network
		int numDestroy; // number of connections to be destroyed
		
		// get Case variables 
		numNet = in.nextInt();
		numNet++;
		numConnect = in.nextInt();
		numDestroy = in.nextInt();
		
		nodes = new int[numNet];
		Arrays.fill(nodes, 1);

		
		// store pairs 
		 ArrayList<IntPair> setPairs = new ArrayList<IntPair>();
		for(int i =0; i < numConnect; i++) {
			int first = in.nextInt();
			int second = in.nextInt();
			setPairs.add(new IntPair(first,second,true));
		}
		//store pairs to destroy
		ArrayList<Integer> destroyPairs = new ArrayList<Integer>();
		for(int j =0 ; j<numDestroy;j++) {
			int tmp =in.nextInt();
			destroyPairs.add(tmp);
			int f = setPairs.get(tmp-1).one;
			int s = setPairs.get(tmp-1).two;
			setPairs.set(tmp-1,new IntPair(f,s,false));
		}
		
		//store disjoint set
		Destroy set = new Destroy(numNet);
		for(int k=0; k<numConnect; k++) {
			if(setPairs.get(k).use) {
			set.union(destroyPairs, setPairs,k);
			}
		}
		initializeSize(numNet);
		
		// Add each pair to be destroyed one at a time
		for(int k=numDestroy-1; k>-1; k--) {
			int tmp = destroyPairs.get(k);
			boolean res = set.union(destroyPairs, setPairs,tmp-1);
			if(res == true) {
				calculate(numNet);
			}
			else {
			end.add(size);
			}
		}
		
		for(int m = end.size()-1; m>-1; m--) { 
			System.out.print(end.get(m) + "\n");
		}
	}
	
}

class IntPair{
	  public int one;
	  public int two;
	  public boolean use;
	  
	  IntPair(int newOne, int newTwo, boolean newUse){
		  one= newOne;
		  two = newTwo;
		  use = newUse;
	  }
	  
	  public  void setOne(int newOne) {
		  one = newOne;
	  }
	  
	  public  void setTwo(int newTwo) {
		  one = newTwo;
	  }
	  public  void setTwo(boolean newUse) {
		  use = newUse;
	  }
	  public int getOne() {
			return one;
		}
	  public int getTwo() {
			return two;
		}
	  public boolean getUse() {
			return use;
		}

}
