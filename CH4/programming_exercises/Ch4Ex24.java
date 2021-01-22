import java.util.Scanner;
import java.util.concurrent.*;

public class Ch4Ex24 {

    private static class PiCalc implements Callable<Integer> {
        private final int num;

        public PiCalc(int num) {
            this.num = num;
        }

        private double getRandomDouble() {
            return ThreadLocalRandom.current().nextDouble(-1, 1);
        }

        @Override
        public Integer call() {
            int res = 0;
            for (int i = 0; i < this.num; i++) {
                double x = getRandomDouble();
                double y = getRandomDouble();

                double dist = Math.sqrt(x * x + y * y);
                if (dist <= 1) {
                    res++;
                }
            }
            return res;
        }
    }

    public static void main(String[] args) {
        System.out.print("num: ");
        Scanner sc = new Scanner(System.in);
        int num = sc.nextInt();
        ExecutorService service = Executors.newSingleThreadExecutor();
        Future<Integer> iFuturePts = service.submit(new PiCalc(num));
        try {
            System.out.println(iFuturePts.get());
            System.out.println("PI = " + 4 * (double)iFuturePts.get() / num);
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
            service.shutdown();
        }
        service.shutdown();
    }
}
