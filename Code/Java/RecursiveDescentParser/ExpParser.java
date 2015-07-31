package RecursiveDescentParser;
import java.util.*;

/*
Expression example: "pop. > 500000 and elevation > = 150  and elevation < = 200".
*/

public class ExpParser {

    public static Token currentToken;
    public static ListIterator<Token> lit;
    public static String error;

    public static AST parse(String exp) throws Exception {
        List<Token> tokens = ExpLexer.tokenize(exp);
        AST tree = ExpParser.parse(tokens);
        if (ExpParser.error != null)
            throw new Exception(ExpParser.error);
        return tree;    
    }
    
    public static AST parse(List<Token> tokens) {
        lit = tokens.listIterator();
        if (!lit.hasNext()) {
            currentToken = new Token(Token.EOF);
        } else currentToken = lit.next();
        AST result = logicalOrExpression();
        eatToken(Token.EOF);
        return result;
    }

    public static void errorExpected(Token foundToken, Object expectedType) {
        ExpParser.error = "Wrong token "+foundToken+", expected <"+expectedType+">.";
    }
        
    public static Token eatToken(String type) {
        Token token = currentToken;
        if (!currentToken.type.equals(type)) {
            errorExpected(currentToken, type);
            return token;
        }
        if ( !lit.hasNext() ) {
            currentToken = new Token(Token.EOF);
            return token;
        }
        currentToken = lit.next();
        return token;
    }
    
    /*
      logicalOrExpression
        :   logicalAndExpression (OR logicalOrExpression)*
        ;  
    */
    public static AST logicalOrExpression() {
        AST result = logicalAndExpression();
        while (currentToken.type == Token.OR) {
            Token token = eatToken(Token.OR);
            AST right = logicalOrExpression();
            result = new AST(AST.LOG, token, result, right);
        }        
        return result;
    }

    /*
      logicalAndExpression
          :   equalityExpression (AND logicalAndExpression)*
          ;
    */
    public static AST logicalAndExpression() {
        AST result = relationalExpression();
        while (currentToken.type == Token.AND) {
            Token token = eatToken(Token.AND);
            AST right = logicalAndExpression();
            result = new AST(AST.LOG, token, result, right);
        }
        return result;
    }

    /*
      relationalExpression
          :   primaryExpression
              (
                  '<'
              |   '>'  
              |   '<=' 
              |   '>=' 
              |   '==' 
              |   '!=' 
              )
              primaryExpression
          ;
    */
    public static AST relationalExpression() {
        AST left = primaryExpression();
        Token token;
        switch(currentToken.type) {
            case Token.LS  : token = eatToken(Token.LS)   ; break;
            case Token.GR  : token = eatToken(Token.GR)   ; break;
            case Token.LEQ : token = eatToken(Token.LEQ)  ; break;
            case Token.GEQ : token = eatToken(Token.GEQ)  ; break;
            case Token.EQ  : token = eatToken(Token.EQ)   ; break;
            case Token.NEQ : token = eatToken(Token.NEQ)  ; break;
            default:
                errorExpected(currentToken, "RalationToken");
                return null;
        }
        AST right = primaryExpression();
        return new AST(AST.CMP, token, left, right);
    }

    /*
      primaryExpression
          :   ID
          |   NUMBER
          ;    
    */
    public static AST primaryExpression() {
        Token token;
        if (currentToken.type == Token.ID)
            token = eatToken(Token.ID);
        else
            token = eatToken(Token.NUMBER);
        return new AST(AST.TERM, token);
    }

}