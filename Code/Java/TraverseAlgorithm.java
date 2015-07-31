import java.util.function.*;
import java.io.*;

abstract public class TraverseAlgorithm {
    
    public class LinePrinter implements AutoCloseable {
    
        public int linesCount = 0;
        public PrintWriter printWriter;
        private ByteArrayOutputStream baos;

        public LinePrinter(PrintWriter printWriter) {
            this.printWriter = printWriter;
        }
        
        public void println(String x) {
            this.printWriter.println(x);
            linesCount++;
        }
        
        public void close() {
            this.printWriter.close();
        }
    
    }

    public Graph graph;
    public LinePrinter linePrinter;

    public Function<Integer, Void> processNodeFunction = noden -> {
        Graph.CityInformation info = this.graph.nodes.get(noden);
        String message = "(City: "+info.city+", Pop: "+info.population+", Elevation: "+info.elevation+")";
        this.linePrinter.println(message);
        System.out.println(message);
        return null;
    };

    public TraverseAlgorithm(Graph graph) {
        this.graph = graph;
        this.linePrinter = new LinePrinter(new PrintWriter(System.out, true));
    }    

    public TraverseAlgorithm(String[] args) throws Exception {
        this(Tool.argsToGraph(args));
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        this.linePrinter = new LinePrinter(new PrintWriter( baos ));
        this.linePrinter.baos = baos;
    }
    
    public void processNode(int noden) {
        this.processNodeFunction.apply(noden);
    }
    
    public void run() throws Exception {
        String algoName = this.getClass().getName();
        Profiler.startTimeProfile("ALGO:"+algoName);
        this.run(this.graph.source);
        Profiler.endTimeProfile  ("ALGO:"+algoName);
        int lines = this.linePrinter.linesCount;
        System.out.println("Nodes visited: "+lines);
        Profiler.startTimeProfile("OUTPUT");
        if (this.linePrinter.baos != null) {
            this.linePrinter.printWriter.flush();
            this.linePrinter.close();
            String result = this.linePrinter.baos.toString();
            PrintWriter outpw = new PrintWriter("Result_of_"+algoName+"_from_Source_to_Target_N_"+lines+".txt");
            outpw.print(result);
            outpw.close();
        }
        Profiler.endTimeProfile("OUTPUT");
        Profiler.writeProfiles("Profile_of_"+algoName+"_from_Source_to_Target_N_"+lines+".txt");
    }
    
    abstract public Boolean run(int from) throws Exception;

}