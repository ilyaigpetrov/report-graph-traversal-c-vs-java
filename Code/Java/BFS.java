import java.util.*;

public class BFS extends TraverseAlgorithm {

    public static void main(String[] args) throws Exception {
        BFS bfs = new BFS(args);
        bfs.run();
    }

    private Set<Integer> visited = new LinkedHashSet<Integer>();
    private Queue<Integer> visitQueue = new LinkedList<Integer>();

    public BFS(String[] args) throws Exception {
        super(args);
    }

    public BFS(Graph graph) {
        super(graph);
    }

    /*
    
      Pseudo code

        choose some starting vertex x
        visit x
        list L = x
        while L nonempty
        choose some vertex v from front of list        
        for each unmarked neighbor w
            visit w
            add w it to end of L
        
    */
    @Override
    public Boolean run(int from) throws Exception {
        this.visitQueue.add(from);
        if (visitAndCheck(from))
            return true;
        while(this.visitQueue.size() != 0) {
            int node = this.visitQueue.remove();
            for( int nbr : graph.getNeighbours(node) ) {
                if (!visited.contains(nbr)) {
                    if (visitAndCheck(nbr))
                        return true;
                    visitQueue.add(nbr);
                }
            }
        }
        return false;
    }

    public Boolean visitAndCheck(int noden) throws Exception {
        this.processNode(noden);
        visited.add(noden);
        return graph.ifTarget(noden);
    }


}