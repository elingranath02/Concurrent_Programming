import java.sql.Timestamp;
import java.util.Date;
import java.util.Random;

import sharedbathroom.SharedBathroom;

public class Threads extends Thread {

    long maxSleep = 10000;
    long maxSleepBathroom = 4000;

    SharedBathroom bathroom;

    public Threads(SharedBathroom bathroom) {
        this.bathroom = bathroom;
    }

    public static void main(String[] args) {
        SharedBathroom  bathroom = new SharedBathroom();

        int maximum_threads = 10;
        for (int i = 0; i < maximum_threads; i++) {
            Threads thread = new Threads(bathroom);
            thread.start();
        }
    }

    public void threadSleep(long sleepTime) {

        Random random = new Random();
        long sleep = 0;
        long min = 2000;
        while (sleep == 0) {
            sleep = random.nextLong(sleepTime);
        }
        try {
            Thread.sleep(sleep + min);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void run() {

        while (true) {

            threadSleep(maxSleep);

            if ((Thread.currentThread().threadId()) % 2 == 0) {
                womanQueue(bathroom);
            } else {
                manQueue(bathroom);
            }
        }
    }

    public void womanQueue(SharedBathroom bathroom) {
        System.out.println("Woman: " + Thread.currentThread().threadId() + " in queue at time: " +
        new Timestamp(new Date().getTime()));
        bathroom.womanEnter();
        threadSleep(maxSleepBathroom);
        bathroom.womenExit();

    }

    public void manQueue(SharedBathroom bathroom) {
        System.out.println("Man: " + Thread.currentThread().threadId() + " in queue at time: " +
        new Timestamp(new Date().getTime()));
        bathroom.manEnter();
        threadSleep(maxSleepBathroom);
        bathroom.manExit();

    }
}