package bsh.util;

import org.gnu.readline.ReadlineCompleter;

/**
 * An adapter for org.gnu.readline's ReadlineCompleter interface to map to
 * BeanShell's NameCompleter interface.
 *
 * @see org.gnu.readline.ReadlineReader
 * @version $Revision$
 * @author Shane Celis <shane@terraspring.com>
 **/
public class BshCompleter implements ReadlineCompleter {

    private NameCompletion completer;

    /**
     * Constructs a <code>ReadlineCompleter</code> out of a
     * <code>NameCompleter</code> object.
     **/
    public BshCompleter(NameCompletion completer) {
        this.completer = completer;
    }

    /**
     * Returns String of completion if unambiguous, otherwise null
     **/
    public String completer(String text, int state) {
        // Not sure what state is used for in ReadlineCompleter
        String[] completions = completer.completeName(text);
        if (completions.length == 1 && state == 0) {
            return completions[0];
        } else {
            return null;        // ambiguous result
        }
    }

}
