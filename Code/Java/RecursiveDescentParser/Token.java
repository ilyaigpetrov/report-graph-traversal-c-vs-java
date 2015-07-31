package RecursiveDescentParser;
public class Token {

    public final static String
        EOF = "EOF",
        AND = "AND",
        OR  = "OR",
        ID  = "ID",
        NUMBER = "NUMBER",
        EQ  = "==",
        NEQ = "!=",
        GR  = ">",
        LS  = "<",
        GEQ = ">=",
        LEQ = "<=";

    public String type;
    public String text = "";
    public String getText() {
        return this.text;
    }
    
    public Token(String type) {
        this.type = type;
    }
    
    public Token(String type, String text) {
        this(type);
        this.text = text;
    }

    public String toString() {
        return "<"+this.type+":"+this.text+">";
    }
}
