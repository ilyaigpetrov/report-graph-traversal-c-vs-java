package RecursiveDescentParser;
import java.util.function.*;
import java.lang.*;

public class AST {

    public String type;
    public final static String
          TERM = "TERM"
        , CMP  = "CMP"
        , LOG  = "LOG"
        ;
    public AST left;
    public AST right;
    public Token token;
    public AST(String type, Token token) {
        this.type = type;
        this.token = token;
    }
    public AST(String type, Token token, AST left, AST right) {
        this(type, token);
        this.left = left;
        this.right = right;
    }

    public interface LookupSignature {
        public ExpResult apply(String value) throws Exception;
    }

    public ExpResult evaluate(LookupSignature lookup) throws Exception {
        switch( this.type ) {
            case AST.LOG:
              Boolean result;
              switch(this.token.type) {
                case Token.OR:
                  return this.left.evaluate(lookup).or( this.right.evaluate(lookup) );
                case Token.AND:
                  return this.left.evaluate(lookup).and( this.right.evaluate(lookup) );
              }
              break;
            case AST.CMP:
                switch(this.token.type) {
                  case Token.EQ:
                    return this.left.evaluate(lookup).equals( this.right.evaluate(lookup) );
                  case Token.NEQ:
                    return this.left.evaluate(lookup).notEquals( this.right.evaluate(lookup) );
                  case Token.GR:
                    return this.left.evaluate(lookup).less( this.right.evaluate(lookup) );
                  case Token.LS:
                    return this.left.evaluate(lookup).greater( this.right.evaluate(lookup) );
                  case Token.GEQ:
                    return this.left.evaluate(lookup).greaterEq( this.right.evaluate(lookup) );
                  case Token.LEQ:
                    return this.left.evaluate(lookup).lessEq( this.right.evaluate(lookup) );
                }
              break;
            case AST.TERM:
              switch( this.token.type ) {
                case Token.ID:
                  return lookup.apply(this.token.text);
                case Token.NUMBER:
                  return new ExpResult( Double.parseDouble(this.token.text) );
              }
              break;
        }
        throw new Exception("Can't evaluate expression.");
    }
    
    public String toString() {
        String result = "["+this.type+"]"+this.token;
        if (this.left != null) {
            result += ":\n{"+this.left.toString()+", "+this.right.toString()+"\n}";
        }
        return result;
    }
    
    
}
