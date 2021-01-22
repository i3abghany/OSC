import java.util.Scanner;
import java.util.concurrent.*;

public class Ch4Ex22 {

    private static class Mean implements Callable<Double> {
        private final int[] arr;

        public Mean(int[] arr) {
            this.arr = arr;
        }

        @Override
        public Double call() throws Exception {
            double res = 0.0d;
            for (int j : arr) res += j;
            res /= arr.length;
            return res;
        }
    }

    private static class Minimum implements Callable<Integer> {
        private final int[] arr;

        public Minimum(int[] arr) {
            this.arr = arr;
        }

        @Override
        public Integer call() throws Exception {
            int m = Integer.MAX_VALUE;
            for (int j : arr) m = Math.min(m, j);
            return m;
        }
    }

    private static class Maximum implements Callable<Integer> {
        private final int[] arr;

        public Maximum(int[] arr) {
            this.arr = arr;
        }

        @Override
        public Integer call() throws Exception {
            int m = Integer.MIN_VALUE;
            for (int j : arr) m = Math.max(m, j);
            return m;
        }
    }
    public static void main(String[] args) {
        System.out.println("num: ");
        Scanner sc = new Scanner(System.in);
        int[] arr = new int[sc.nextInt()];
        for (int i = 0; i < arr.length; i++)
            arr[i] = sc.nextInt();

        ExecutorService service = Executors.newFixedThreadPool(3);
        Future<Double> df = service.submit(new Mean(arr));
        Future<Integer> iFutureMin = service.submit(new Minimum(arr));
        Future<Integer> iFutureMax = service.submit(new Maximum(arr));


        try {
            System.out.println("Mean: " + df.get());
            System.out.println("Minimum: " + iFutureMin.get());
            System.out.println("Maximum: " + iFutureMax.get());
			service.shutdown();
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
		service.shutdown();
    }
}
