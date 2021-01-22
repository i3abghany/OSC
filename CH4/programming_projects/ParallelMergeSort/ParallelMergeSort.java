import java.util.concurrent.*;

public class ParallelMergeSort {

    public static class SortTask extends RecursiveAction {
        private final int[] arr;
        private final int lo;
        private final int hi;
        private final static int THRESHOLD = 128;

        public SortTask(int[] arr, int lo, int hi) {
            this.arr = arr;
            this.lo = lo;
            this.hi = hi;
        }

        @Override
        protected void compute() {
            if (this.hi - this.lo + 1 > THRESHOLD) {
                int mid = this.lo + (this.hi - this.lo) / 2;
                SortTask sortLo = new SortTask(arr, this.lo, mid);
                SortTask sortHi = new SortTask(arr, mid + 1, hi);

                sortLo.fork();
                sortHi.fork();

                sortLo.join();
                sortHi.join();

                MergeTask mergeArrs = new MergeTask(arr, 0, mid, mid + 1, hi);
                ExecutorService s = Executors.newSingleThreadExecutor();
                var temp = s.submit(mergeArrs);
                try {
                    temp.get(); // join.
                    s.shutdown();
                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }

            } else {
                for (int i = lo; i < hi; i++) {
                    int max_idx = i;
                    for (int j = i + 1; j <= hi; j++) {
                        if (arr[j] < arr[max_idx]) {
                            max_idx = j;
                        }
                    }
                    int tmp = arr[i];
                    arr[i] = arr[max_idx];
                    arr[max_idx] = tmp;
                }
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

    public static void main(String[] args) {
        int SIZE = 1024;
        int[] arr = new int[SIZE];

        for (int i = 0; i < arr.length; i++)
            arr[i] = ThreadLocalRandom.current().nextInt(0, 2048);

        SortTask srt = new SortTask(arr, 0, SIZE - 1);
        new ForkJoinPool().invoke(srt);

        for (int i : arr)
            System.out.println(i);
    }
}
