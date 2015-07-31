import java.util.*;

public class DFS extends TraverseAlgorithm {

    public static void main(String[] args) throws Exception {
        DFS dfs = new DFS(args);
        dfs.run();
    }

    private Set<Integer> visited = new LinkedHashSet<Integer>();
    
    public DFS(String[] args) throws Exception {
        super(args);
    }    
    
    public DFS(Graph graph) {
        super(graph);
    }
    
    /*
    
      Pseudo code

        dfs(vertex v)
        {
        visit(v);
        for each neighbor w of v
            if w is unvisited
            {
            dfs(w);
            add edge vw to tree T
            }
        }

    */
    @Override
    public Boolean run(int from) throws Exception {
        visit(from);
        if (graph.ifTarget(from)) {
            return true;
        }
        for( int nbr : graph.getNeighbours(from) ) {
            if (!visited.contains(nbr)) {
                Boolean ifFound = run(nbr);
                if(ifFound)
                    return true;
            }
        }
        return false;
    }

    public void visit(int noden) {
        this.processNode(noden);
        visited.add(noden);
    }


}