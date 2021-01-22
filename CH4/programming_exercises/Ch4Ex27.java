import java.util.Scanner;

public class Ch4Ex26 {

    private static class ThreadTask implements Runnable {
        private int[] fib;

        public ThreadTask(int[] f) {
            this.fib = f;
        }

        @Override
        public void run() {
            this.fib[0] = 0;
            this.fib[1] = 1;

            for (int i = 2; i < fib.length; i++) {
                this.fib[i] = this.fib[i - 1] + this.fib[i - 2];
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        System.out.print("Num: ");
        Scanner sc = new Scanner(System.in);
        int SIZE = sc.nextInt();
        var fibs = new int[SIZE];
        Thread th = new Thread(new ThreadTask(fibs));
        th.start();
        th.join();


        for (int el : fibs) {
            System.out.println(el);
        }
    }
}
