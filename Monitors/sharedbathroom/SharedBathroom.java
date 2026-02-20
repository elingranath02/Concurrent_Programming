package sharedbathroom;
import java.sql.Timestamp;
import java.util.Date;

public class SharedBathroom {
    private volatile boolean womenTurn;
    private volatile boolean menTurn;
    private volatile int nrOfWomenInBathroom;
    private volatile int nrOfMenInBathroom;
    private volatile int womenInQueue;
    private volatile int menInQueue;


    public SharedBathroom() {
        this.womenTurn = false;
        this.menTurn = false;
        this.nrOfWomenInBathroom = 0;
        this.nrOfMenInBathroom = 0;
        this.womenInQueue = 0;
        this.menInQueue = 0;
    }
    
    public synchronized void womanEnter() {
        if(nrOfWomenInBathroom > 0 && menInQueue > 0){
            menTurn = true;
        }else womenTurn = true;

        while (menTurn) {
            try {
                womenInQueue++;
                wait();
                womenInQueue--;
            } catch (InterruptedException e) {
            }
        }
        nrOfWomenInBathroom++;
        System.out.println("Woman: " + Thread.currentThread().threadId() + " enters bathroom at time: " +
        new Timestamp(new Date().getTime()));
    }

    public synchronized void manEnter() {
        if(nrOfMenInBathroom > 0 && womenInQueue > 0){
            womenTurn = true;
        }else menTurn = true;

        while (womenTurn) {
            try {
                menInQueue++;
                wait();
                menInQueue--;
            } catch (InterruptedException e) {
            }
        }
        nrOfMenInBathroom++;
        System.out.println("Man: " + Thread.currentThread().threadId() + " enters bathroom at time: " +
        new Timestamp(new Date().getTime()));
    }

    public synchronized void womenExit() {
        nrOfWomenInBathroom--;
        System.out.println("Woman: " + Thread.currentThread().threadId() + " exits bathroom at time: " +
        new Timestamp(new Date().getTime()));

        if (nrOfWomenInBathroom == 0 && menInQueue > 0){
            System.out.println();
            System.out.println("Nr of women in bathroom: " + nrOfWomenInBathroom);
            System.out.println("Nr of women waiting: " + womenInQueue);
            System.out.println("Nr of men waiting: " + menInQueue);
            System.out.println();

            menTurn = true;
            womenTurn = false;
            notifyAll();
        }
    }

    public synchronized void manExit() {
        nrOfMenInBathroom--;
        System.out.println("Man: " + Thread.currentThread().threadId() + " exits bathroom at time: " +
        new Timestamp(new Date().getTime()));
        
        if (nrOfMenInBathroom == 0 && womenInQueue > 0) {
            System.out.println();
            System.out.println("Nr of men in bathroom: " + nrOfMenInBathroom);
            System.out.println("Nr of men waiting: " + menInQueue);
            System.out.println("Nr of women waiting: " + womenInQueue);
            System.out.println();

            womenTurn = true;
            menTurn = false;
            notifyAll();
            
        }
    }
}