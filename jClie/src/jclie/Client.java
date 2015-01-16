/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package jclie;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

/**
 *
 * @author cmo
 */
public class Client {
    //Constants
    private static final int PORT_NUMBER = 5976;
    private static final int CHUNK_SIZE = 1024;
    private static final int OFF_SET = 0;

    //Variables
    String inIPAssress, inFileLocation;
    Socket soc;	       // Socket used to connect to Server
    FileInputStream inFile;   // Used to read the file.
    InputStream in;            // Used to read data from socket
    OutputStream out;          // Used to write data to socket
    File file;

    /**
     * Creates new form Prog3Client
     * @param ipAddress
     * @param file
     */
    public Client(String ipAddress, String file) {
        inIPAssress = ipAddress;
        inFileLocation = file;
        System.out.println("Reseaved: " + ipAddress + ", " + file);
    }
    
    public void SendFile() {
        try {
            System.out.println("Opening file...");
            file = new File(inFileLocation);
            if (file != null) {
                if (connect()) {
                    sendNullTerminatedString(file.getName());
                    System.out.println("File Name: " + file.getName() + "\n");
                    sendNullTerminatedString(Long.toString(file.length()));
                    System.out.println("File Size: " + Long.toString(file.length()) + "\n");
                    System.out.println("Sending File...\n");
                    sendFile(inFileLocation);
                    System.out.println("Disconnected from server.");
                    disconnect();
                }
            } else {
                System.out.println("No file selected.\n");
            }
        } catch (IOException ex) {
            System.out.println(ex.getMessage() + '\n');
        }
    }                                                                                   
                  
    /**
     * Method connect():
     *
     * Requests a connection with the server.
     */
    private boolean connect() throws IOException {
        boolean t = false;
        while (!InetAddress.getByName(inIPAssress).isReachable(PORT_NUMBER)){
            System.out.println("Checking connection again...");
        }
        if (InetAddress.getByName(inIPAssress).isReachable(PORT_NUMBER)) {
            soc = new Socket(inIPAssress, PORT_NUMBER);
            out = soc.getOutputStream();
            in = soc.getInputStream();
            System.out.println("Connected to Server at "
                    + soc.getInetAddress().getHostAddress() + '\n');
            t = true;
        } else {
            System.out.println("Server cannot be found.\n");
        }
        return t;
    }

    /**
     * Method disconnect():
     *
     * This function disconnects the socket.
     *
     * @throws IOException
     */
    private void disconnect() throws IOException {
        in.close();
        out.close();
        soc.close();
    }

    /**
     * Method sendNullTerminatedString(String):
     *
     * This method takes a String s (either a file name or a file size,) as a
     * parameter, turns String s into a sequence of bytes ( byte[] ) by calling
     * getBytes() method, and sends the sequence of bytes to the Server. A null
     * character ‘\0’ is sent to the Server right after the byte sequence.
     */
    private void sendNullTerminatedString(String s) {
        try {
            //byte[] n = new byte[CHUNK_SIZE];
            out.write(s.getBytes());
            out.write('\0');
        } catch (IOException ex) {
            System.out.println(ex.getMessage() + '\n');
        }
    }

    /**
     * Method sendFile(String):
     *
     * This method takes a full-path file name, decomposes the file into smaller
     * chunks (each with 1024 bytes), and sends the chunks one by one to the
     * Server (loop until all bytes are sent.) A null character ‘\0’ is sent to
     * the Server right after the whole file is sent.
     */
    private void sendFile(String fullPathFileName) {
        //use the FileInputStream class to read a binary file 
        //set up a loop to repeatedly read and write chunks 
        try {
            byte[] n = new byte[CHUNK_SIZE];
            inFile = new FileInputStream(fullPathFileName);
            int count;
            count = inFile.read(n);
            while (count != -1) {
                out.write(n, OFF_SET, count);
                count = inFile.read(n);
            }
            validating();
        } catch (IOException ex) {
            System.out.println(ex.getMessage() + '\n');
        }
    }

    /**
     * Method validating():
     *
     * Reads the message from the server after the file is uploaded.
     */
    private void validating() {
        try {
            byte[] b = new byte[CHUNK_SIZE];
            in.read(b);
            boolean success = false;
            for (int i = 0; i < b.length; i++) {
                byte accept = b[i];
                if (accept == '@') {
                    success = true;
                    i = b.length;
                }
            }
            if (success) {
                System.out.println("File Uploaded Successfully.\n");
            } else {
                System.out.println("File Upload Failed.\n");
            }
        } catch (IOException ex) {
            System.out.println(ex.getMessage() + "\n");
        }
    }
}