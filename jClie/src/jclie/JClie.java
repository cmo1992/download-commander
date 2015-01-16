package jclie;

/**
 *
 * @author cmo
 */

public class JClie {
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Client Started.\n");
        System.out.println("Args: " + args[0] + ", " + args[1]);
        Client c = new Client(args[0], args[1]);
        c.SendFile();
    }

}
