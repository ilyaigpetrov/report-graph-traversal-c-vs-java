import java.util.*;
import java.util.regex.*;
import java.io.*;
import java.util.function.*;

import RecursiveDescentParser.*;

public class Graph {
    public Map<Integer, CityInformation> nodes;
    public Map<Integer, LinkedHashSet<Integer>> edges;
    public int count;
    public int source;
    public int target = -1;
    
    public interface ifTargetSignature {
        public Boolean apply(int node) throws Exception;
    }
    
    private ifTargetSignature _ifTargetFunction = noden -> noden == this.target;

    public Boolean ifTarget(int noden) throws Exception {
        return this._ifTargetFunction.apply(noden);
    }

    public Graph(Integer n) {
        this.nodes = new HashMap<Integer, CityInformation>(n);
        this.edges = new HashMap<Integer, LinkedHashSet<Integer>>(n);
        this.count = n;
    }
    
    public LinkedHashSet<Integer> getNeighbours(int from) {
        LinkedHashSet<Integer> set = this.edges.get(from);
        if (set == null) {
            set = new LinkedHashSet<Integer>();
            this.edges.put( from, set );
        }
        return set;
    }
    
    public int countNodes() {
        return this.nodes.size();
    }
    
    public void addEdge(int from, int to) {
        LinkedHashSet<Integer> nbrs = this.edges.get( from );
        if (nbrs == null) {
            nbrs = new LinkedHashSet<Integer>(); // Ordered.
            this.edges.put( from, nbrs );
        }
        nbrs.add( to );
    }

    public void populateCityInformation(String line) {
        if (Pattern.compile("\\s*").matcher(line).matches())
            return; // silently swallow empty line.
        // e.g. 2	Quebec City	516622			-98.05
        Pattern pattern = Pattern.compile("\\s* (?<number> \\d+ ) \\s+ (?<city> \\D+? ) \\s+ (?<population> \\d+ ) \\s+ (?<elevation> -?\\d+ ) \\s*", Pattern.COMMENTS);
        Matcher matcher = pattern.matcher(line);
        if ( matcher.find() ) {
            Integer number = Integer.parseInt( matcher.group("number") );
            String city = matcher.group("city");
            Integer population = Integer.parseInt( matcher.group("population") );
            Double elevation = Double.parseDouble( matcher.group("elevation") );
            CityInformation info = new CityInformation(city, population, elevation);
            this.nodes.put( number, info );
        } else throw new IllegalArgumentException("Can't populate from graph from line:"+line+"!");
    }
    
    public void populateEdges(BufferedReader br) throws IOException {
        System.out.println("Reading adjacency matrix...");
        String line;
        //Pattern pattern = Pattern.compile("\\s* ( \\d+ ) \\s*", Pattern.COMMENTS);
        int currentLine = 0;
        while ((line = br.readLine()) != null) {
            System.out.println("Line "+currentLine+" of adj");
            //Matcher matcher = pattern.matcher(line);
            int toi = 0;
            int i = 0;
            for (; toi < this.count; toi++ ) {
                char ch = line.charAt(i++);
                while (ch == ' ' || ch == '\t')
                    ch = line.charAt(i++);
                Boolean ifAdj = ch == '1';
                if (ifAdj) {
                    int from = currentLine + 1;
                    int to = toi + 1;
                    this.addEdge(from, to);
                } else if (ch != '0')
                    throw new IllegalArgumentException("Character "+ch+" on line "+currentLine+" can't be parsed.");
            } 
            //if (toi != this.count) {
            //    throw new IllegalArgumentException("Number of matched columns is not "+this.count+" but "+i+" on line "+currentLine+": "+line+"!");
            //}
            currentLine++;
        }
        br.close();
        System.out.println("finished");
    }

    public ExpResult lookupVariable(int noden, String name) throws Exception {
        CityInformation info = this.nodes.get(noden);
        if (name.endsWith("."))
            name = name.substring(0, name.length() - 1);
        if ("population".startsWith(name))
            name = "population";
        else if ("elevation".startsWith(name))
            name = "elevation";
        else if ("city".startsWith(name))
            name = "city";
        switch(name) {
            case "population":
                return new ExpResult( new Double(info.population) );
            case "elevation":
                return new ExpResult(info.elevation);
            case "city":
                return new ExpResult(info.city.toLowerCase());
            default:
                return new ExpResult(name.toLowerCase()); // City name
        }
    }
    
    public void populateTask(BufferedReader br, Boolean ifIgnoreTarget) throws Exception {
        br.readLine(); // Throw away header.
        String line = br.readLine();
        if (line == null) {
            this.target = -1;
            this.source = 1;
            return;
        }
        // E.g.    1               "pop. > 500000 and elevation > = 150  and elevation < = 200"         
        Pattern pattern = Pattern.compile("\\s* ( \\d+ ) \\s+ ( .+ )", Pattern.COMMENTS);
        Matcher matcher = pattern.matcher(line);
        if ( matcher.matches() ) {
            this.source = Integer.parseInt( matcher.group(1) );
            if (ifIgnoreTarget)
                return;
            String target = matcher.group(2).trim();
            if (target.startsWith("\"")) {
                String expression = target.replaceAll("^\"|\"$", "");
                AST ast = ExpParser.parse(expression);
                this._ifTargetFunction =
                        noden -> {
                            ExpResult result = ast.evaluate(
                                varName -> lookupVariable(noden, varName)
                            );
                            return result.ifTrue();
                        };
                return;
            }
            this.target = Integer.parseInt( target );
        } else {
            throw new IllegalArgumentException("Incorrect target on line:"+line);
        }
    }

    public class Node {
        public Integer number;
        public CityInformation information;

        public Node(Integer number, CityInformation info) {
            this.number = number;
            this.information = info;
        }
        
        @Override
        public boolean equals(Object o) {
            if(!(o instanceof Node)) return false;
            Node other = (Node) o;
            return this.number == other.number;
        }

        @Override
        public int hashCode() {
          return number.hashCode();
        }
        
    }

    public class CityInformation {
        public String city;
        public Integer population;
        public Double elevation;
        
        public CityInformation(String city, Integer population, Double elevation) {
            this.city = city;
            this.population = population;
            this.elevation = elevation;
        }
    }
}