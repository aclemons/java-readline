package samer.silk;
import java.util.*;
import org.gnu.readline.*;
import jsint.*;

/**
	This completer works with the JScheme scheme
	interpreter by looking up symbols in the global
	namespace. You can register it with the Java
	Readline library either in Java or in scheme. Something
	like the following should do it:

	<pre>
	;;; Switches JScheme's input port to a new one that uses
	;;; a ReadlineReader with a ">" prompt.
	;;; (Note that on some systems, you might get a double
	;;; prompt "> >" because JScheme's main loop also prints
	;;; a prompt. If this bugs you a lot, you can just write your
	;;; own main loop to replace jsint.Scheme.readEvalWriteLoop().)
	(set! jsint.Scheme.input$ (jsint.InputPort.
		(org.gnu.readline.ReadlineReader. "> "
			org.gnu.readline.ReadlineLibrary.GnuReadline$)))

   	;;; not sure that this helps a lot, but I suppose it's good practice...
	(.addShutdownHook (Runtime.getRuntime)
		(Thread. (lambda ()
			(display "cleaning up readline.\n")
			(org.gnu.readline.Readline.cleanup))))

   	;;; set Readline's completer to a new Scheme-aware one
	(org.gnu.readline.Readline.setCompleter (samer.silk.SilkCompleter.))
	</pre>

	Author:  Samer Abdallah (samer.abdallah@elec.qmul.ac.uk)
	November 2002
 */

public class SilkCompleter implements ReadlineCompleter {
	Iterator	it;	// iterates over symbols in JScheme hashtable

	/** Implementation of abstract function. Finds
		possible completions of the given text. */

	public String completer(String text, int state)
	{
		if (state == 0) { // first call to completer(): initialize iterator
			// it=Symbol.symbolTable.entrySet().iterator();
			it=Symbol.symbolTable.values().iterator();
		}
		while (it.hasNext()) { // subsequent calls
			// Map.Entry entry = (Map.Entry)it.next();
			// Symbol symbol=(Symbol)entry.getValue();
			// String	name=(String)entry.getKey();
			Symbol symbol=(Symbol)it.next();
			String name=(String)symbol.toString();

			// Jscheme seems to keep a lot of undefined symbols
			// in the table---pretty much anything you type, actually.
			// so we check and only return defined symbols.
			if (name.startsWith(text) && symbol.isDefined()) return name;
		}
		return null; // we reached the last choice.
	}
}