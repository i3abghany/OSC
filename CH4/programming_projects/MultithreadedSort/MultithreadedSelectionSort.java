import java.util.concurrent.*;

public class MultithreadedSelectionSort {

    public static class SortTask implements Runnable {
        private final int[] arr;
        private final int lo;
        private final int hi;

        public SortTask(int[] arr, int lo, int hi) {
            this.arr = arr;
            this.lo = lo;
            this.hi = hi;
        }

        @Override
        public void run() {
            for (int i = lo; i <= hi; i++) {
                int max_val = Integer.MAX_VALUE;
                int max_idx = lo;
                for (int j = i; j <= hi; j++) {
                    if (arr[j] < max_val) {
                        max_val = arr[j];
                        max_idx = j;
                    }
                }
                int tmp = arr[i];
                arr[i] = arr[max_idx];
                arr[max_idx] = tmp;
            }
        }
    }

    public static class MergeTask implements Runnable {
        private final int[] arr;
        private final int lo1, hi1;
        private final int lo2, hi2;

        public MergeTask(int[] arr, int lo1, int hi1, int lo2, int hi2) {
            this.arr = arr;

            this.lo1 = lo1;
            this.hi1 = hi1;

            this.lo2 = lo2;
            this.hi2 = hi2;
        }

        @Override
        public void run() {
            int[] loArr = new int[hi1 - lo1 + 1];
            int[] hiArr = new int[hi2 - lo2 + 1];

            System.arraycopy(arr, lo1, loArr, 0, hi1 - lo1 + 1);
            System.arraycopy(arr, lo2, hiArr, 0, hi2 - lo2 + 1);

            int ptrArr = lo1;
            int ptr1 = 0;
            int ptr2 = 0;

            while (ptrArr < hi2) {
                if (ptr1 == loArr.length) break;
                if (ptr2 == hiArr.length) break;
                if (loArr[ptr1] < hiArr[ptr2]) {
                    arr[ptrArr++] = loArr[ptr1++];
                } else {
                    arr[ptrArr++] = hiArr[ptr2++];
                }
            }

            while (ptr1 < loArr.length)
                arr[ptrArr++] = loArr[ptr1++];

            while (ptr2 < hiArr.length)
                arr[ptrArr++] = hiArr[ptr2++];
        }
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        int SIZE = 16;
        int[] arr = new int[SIZE];

        for (int i = 0; i < arr.length; i++)
            arr[i] = ThreadLocalRandom.current().nextInt(0, 32);

        ExecutorService p = Executors.newFixedThreadPool(3);
        var z1 = p.submit(new SortTask(arr, 0, SIZE / 2 - 1));
        var z2 = p.submit(new SortTask(arr, SIZE / 2, SIZE - 1));
        var z3 = p.submit(new MergeTask(arr, 0, SIZE / 2 - 1, SIZE / 2, SIZE - 1));

        z1.get();
        z2.get();
        z3.get();

        for (int i : arr)
            System.out.println(i);

        p.shutdown();
    }
}
