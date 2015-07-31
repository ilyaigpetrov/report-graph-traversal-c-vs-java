package RecursiveDescentParser;
import java.util.regex.*;
import java.util.*;

public class ExpLexer {

    private static Token tokenizePrimaryExp(String exp) {
        String text = exp.trim();
        String modified = text;
        if (text.startsWith("-") && text.length() > 1 )
            modified = modified.substring(1);
        if (Character.isDigit( modified.charAt(0) ))
            return new Token(Token.NUMBER, text);
        return new Token(Token.ID, text);
    }
    
    private static Token tokenizeComparator(String exp) throws Exception {
        String cmp = exp.replaceAll(" ","");
        switch(cmp) {
            case "!=":
                return new Token(Token.NEQ);
            case "==":
                return new Token(Token.EQ);
            case ">":
                return new Token(Token.GR);
            case "<":
                return new Token(Token.LS);
            case ">=":
                return new Token(Token.GEQ);
            case "<=":
                return new Token(Token.LEQ);
            default:
                throw new Exception("Can't recognize "+cmp+" as compare token.");
        }
    }

    public static List<Token> tokenize(String exp) throws Exception {
        List<Token> tokens = new ArrayList<Token>();
        String idOrNumber = "( [a-z.' ]+ | -?[0-9.]+ )";
        String equalityExpression = idOrNumber+" \\s* ([<>=\\ ]+) \\s* "+idOrNumber;
        Pattern pattern = Pattern.compile("\\s* (and | or | && | \\|\\|)? \\s* "+equalityExpression+" \\s*", Pattern.COMMENTS);
        Matcher matcher = pattern.matcher(exp.toLowerCase());
        Boolean ifMatched = false;
        while ( matcher.find() ) {
            ifMatched = true;
            String andOr = matcher.group(1);
            if (andOr != null) {
                if (andOr.charAt(0) == 'a' || andOr.charAt(0) == '&')
                    tokens.add( new Token(Token.AND) );
                else tokens.add( new Token(Token.OR) );
            }
            String left = matcher.group(2);
            String cmp = matcher.group(3);
            String right = matcher.group(4);
            tokens.add( tokenizePrimaryExp(left) );
            tokens.add( tokenizeComparator(cmp) );
            tokens.add( tokenizePrimaryExp(right) );
        }
        if (!ifMatched) {
            throw new Exception("This expression is wrong, can't tokenize it: "+exp+".");
        }
        return tokens;
    }
}