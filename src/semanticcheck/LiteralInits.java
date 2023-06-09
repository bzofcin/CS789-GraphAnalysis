package semanticcheck;

import utilities.*;
import ast.*;

/**
 * 
 * @author Matt Pedersen
 * @version 02/16/2019
 * @since 1.2
 */
public class LiteralInits extends Visitor<Object> {
    Literal insideLiteral = null;

    public Object visitChannelReadExpr(ChannelReadExpr cre) {
        if (insideLiteral != null) {
            PJBugManager.INSTANCE.reportMessage(new PJMessage.Builder()
                    .addAST(cre)
                    .addError(VisitorMessageNumber.SEMATIC_CHECKS_900)
//                    .addArguments(cre)
                    .build());
        }
        return null;
    }

    public Object visitArrayLiteral(ArrayLiteral al) {
        Literal insideLiteralOld = insideLiteral;
        insideLiteral = al;
        super.visitArrayLiteral(al);
        insideLiteral = insideLiteralOld;
        return null;
    }

    public Object visitRecordLiteral(RecordLiteral rl) {
        Literal insideLiteralOld = insideLiteral;
        insideLiteral = rl;
        super.visitRecordLiteral(rl);
        insideLiteral = insideLiteralOld;
        return null;
    }

    public Object visitProtocolLiteral(ProtocolLiteral pl) {
        Literal insideLiteralOld = insideLiteral;
        insideLiteral = pl;
        super.visitProtocolLiteral(pl);
        insideLiteral = insideLiteralOld;
        return null;
    }

}