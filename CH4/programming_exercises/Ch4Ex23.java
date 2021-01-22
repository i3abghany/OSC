import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.*;

public class Ch4Ex23 {

    private static class Prime implements Callable<ArrayList<Integer>> {
        private final int num;

        public Prime(int num) {
            this.num = num;
        }

        private boolean isPrime(int n) {
            for (int i = 2; i * i <= n; i++) {
                if (n % i == 0) return false;
            }
            return true;
        }

        @Override
        public ArrayList<Integer> call() {
            ArrayList<Integer> res = new ArrayList<>();
            for (int i = 2; i < num; i++) {
                if (isPrime(i)) res.add(i);
            }
            return res;
        }
    }

    public static void main(String[] args) {
        System.out.print("num: ");
        Scanner sc = new Scanner(System.in);
        int num = sc.nextInt();
        ExecutorService service = Executors.newSingleThreadExecutor();
        Future<ArrayList<Integer>> iFutureArr = service.submit(new Prime(num));
        try {
            for (int i : iFutureArr.get()) {
                System.out.println(i);
            }
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
            service.shutdown();
        }

        service.shutdown();
    }
}
