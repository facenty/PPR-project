package First;

import java.io.*;
import java.net.*;
import java.util.Scanner;

public final class Program {
    protected static final String ANSI_RESET = "\u001B[0m";
    protected static final String ANSI_BLACK = "\u001B[30m";
    protected static final String ANSI_RED = "\u001B[31m";
    protected static final String ANSI_GREEN = "\u001B[32m";
    protected static final String ANSI_YELLOW = "\u001B[33m";
    protected static final String ANSI_BLUE = "\u001B[34m";
    protected static final String ANSI_PURPLE = "\u001B[35m";
    protected static final String ANSI_CYAN = "\u001B[36m";
    protected static final String ANSI_WHITE = "\u001B[37m";

    private int portNumber = -1;
    private InetAddress ipAddress;
    private BufferedReader bufferedReader;


    public Program () {}

    public void run()
    {
        getPortAndAddress();

        try {
            createStream(selectInputType());
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        try {
            startSendingData();
        } catch (IOException e) {
            e.printStackTrace();
        }

        onExit();
    }

    private void onExit() {
        bufferedReader = null;
    }

    private void getPortAndAddress()
    {
        Scanner scanner = new Scanner(System.in);
        System.out.println(ANSI_RED + "Write ip address: " + ANSI_RESET);
        while(true) {
            String ipAddressString = scanner.nextLine();
            if(!(ipAddressString.matches("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)")))
            {
                System.out.println("Wrong ip address, try again: ");
                continue;
            }
            try {
                ipAddress = InetAddress.getByName(ipAddressString);
            } catch (UnknownHostException e) {
                e.printStackTrace();
            }
            break;
        }

        System.out.println(ANSI_RED + "Write portno: " + ANSI_RESET);
        while(true) {
            String input = scanner.nextLine();
            if(!(input.matches("[0-9]+"))) {
                System.out.println("Wrong port number, only number. Try again: ");
                continue;
            }
            portNumber = Integer.parseInt(input);
            if (portNumber < 1 || portNumber > 65535)
            {
                System.out.println("Wrong port number, must be between <1, 65535>. Try again: ");
                continue;
            }
            break;
        }
    }

    private String getFileName()
    {
        Scanner scanner = new Scanner(System.in);
        System.out.println(ANSI_RED + "Write file name: " + ANSI_RESET);
        return scanner.nextLine();
    }

    private InputType selectInputType()
    {
        int input = 0;
        do
        {
            System.out.println(ANSI_RED + "Select input type: " + ANSI_RESET);
            System.out.println(ANSI_GREEN + "FILE\t ====> \t1"+ ANSI_RESET);
            System.out.println(ANSI_PURPLE + "KEYBOARD ====> \t2"+ ANSI_RESET);
            System.out.println(ANSI_YELLOW + "RANDOM\t ====> \t3"+ ANSI_RESET);
            System.out.println("Your choice: \t");
            Scanner scanner = new Scanner(System.in);
            input = scanner.nextInt();
            System.out.flush();
        }while((input <1) || (input > 3));

        if(input == 1)
           return InputType.FILE;
        else if(input == 2)
            return InputType.KEYBOARD;
        else
            return InputType.RANDOM;
    }

    private void createStream(InputType inputType) throws FileNotFoundException {
        switch (inputType)
        {
            case FILE:
                bufferedReader = new BufferedReader(new InputStreamReader(new FileInputStream(getFileName())));
                break;
            case KEYBOARD:
                bufferedReader = new BufferedReader(new InputStreamReader(System.in));
                break;
            case RANDOM:
                bufferedReader = new BufferedReader(new InputStreamReader(new FileInputStream("/dev/urandom")));
                break;
        }
    }


    private void startSendingData() throws IOException {
        if ((bufferedReader == null) || (ipAddress == null) || (portNumber == -1))
            return;
        String data;

        DatagramSocket socket = new DatagramSocket();
        while((data = bufferedReader.readLine()) != null) {
            byte[] byteData = data.getBytes();
            DatagramPacket dp = new DatagramPacket(byteData, byteData.length, ipAddress, portNumber);
            socket.send(dp);
        }
        socket.close();

//        while((data = bufferedReader.readLine()) != null)
//        {
//            System.out.println(data);
//        }
    }


}
