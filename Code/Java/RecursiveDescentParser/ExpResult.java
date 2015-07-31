package RecursiveDescentParser;

public class ExpResult implements Comparable<ExpResult> {

    public enum Type {
        STRING, DOUBLE, BOOL
    }
            
    String stringValue = "";
    Double doubleValue = 0.0;
    Boolean boolValue = false;
    Type type;
    
    public ExpResult(Double dv) {
        this.doubleValue = dv;
        this.type = Type.DOUBLE;
    }
    
    public ExpResult(String sv) {
        this.stringValue = sv;
        this.type = Type.STRING;
    }
    
    public ExpResult(Boolean bv) {
        this.type = Type.BOOL;
        this.boolValue = bv;
    }    
            
    public Boolean ifTrue() {
        assert(this.type == Type.BOOL);
        return this.boolValue == true;
    }
    
    public Boolean ifFalse() {
        assert(this.type == Type.BOOL);
        return this.boolValue == false;
    }        

    @Override
    public int compareTo(ExpResult other) {
        if (this.type != other.type) {
            throw new RuntimeException("Can't compare "+this+" and "+other+"!");
        }
        switch(other.type) {
            case STRING:
                return this.stringValue.compareTo(other.stringValue);
            case DOUBLE:
                return this.doubleValue.compareTo(other.doubleValue);
            case BOOL:
                return this.boolValue.compareTo(other.boolValue);
            default:
                throw new RuntimeException("Can't compare with unknown type:["+this.stringValue+","+this.doubleValue+"]");
        }
    }
    
    public ExpResult equals(ExpResult other) {
        return this.compareTo(other) == 0 ? new ExpResult(true) : new ExpResult(false);
    }
    
    public ExpResult notEquals(ExpResult other) {
        return this.compareTo(other) != 0 ? new ExpResult(true) : new ExpResult(false);
    }        
    
    public ExpResult less(ExpResult other) {
        return this.compareTo(other) < 0 ? new ExpResult(true) : new ExpResult(false);
    }
    
    public ExpResult lessEq(ExpResult other) {
        return this.compareTo(other) <= 0 ? new ExpResult(true) : new ExpResult(false);
    }        
    
    public ExpResult greater(ExpResult other) {
        return this.compareTo(other) > 0 ? new ExpResult(true) : new ExpResult(false);
    }
    
    public ExpResult greaterEq(ExpResult other) {
        return this.compareTo(other) >= 0 ? new ExpResult(true) : new ExpResult(false);
    }
    
    public ExpResult or(ExpResult other) {
        Boolean result = this.ifTrue() || other.ifTrue();
        return result ? new ExpResult(true) : new ExpResult(false);
    }
    
    public ExpResult and(ExpResult other) {
        Boolean result = this.ifTrue() && other.ifTrue();
        return result ? new ExpResult(true) : new ExpResult(false);
    }
            
    public String toString() {
        switch(this.type) {
            case STRING:
                return this.stringValue.toString();
            case DOUBLE:
                return this.doubleValue.toString();
            case BOOL:
                return this.boolValue.toString();
            default:
                return "<UNKNOWN TYPE>";
        }
    }
}