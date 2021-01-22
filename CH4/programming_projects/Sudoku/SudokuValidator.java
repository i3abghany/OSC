import java.util.*;
import java.util.concurrent.*;

public class SudokuValidator {

    static class ColumnTask implements Callable<Boolean> {
        private final int[][] board;
        private final int col;

        public ColumnTask(int[][] board, int col) {
            this.board = board;
            this.col = col;
        }

        @Override
        public Boolean call() throws Exception {
            Set<Integer> vis = new TreeSet<>();
            for (int[] ints : board) {
                if (vis.contains(ints[this.col])) {
                    return false;
                } else {
                    vis.add(ints[this.col]);
                }
            }
            return true;
        }
    }

    static class RowTask implements Callable<Boolean> {
        private final int[][] board;
        private final int row;

        public RowTask(int[][] board, int row) {
            this.board = board;
            this.row = row;
        }

        @Override
        public Boolean call() {
            Set<Integer> vis = new TreeSet<>();
            for (int i = 0; i < board[0].length; i++) {
                if (vis.contains(board[this.row][i])) {
                    return false;
                } else {
                    vis.add(board[this.row][i]);
                }
            }
            return true;
        }
    }


    static class BoxTask implements Callable<Boolean> {
        private final int[][] board;
        private final int box;
        private final int BOX_DIMENSION = 3;

        private static Map<Integer, Integer> xBoxMap;
        private static Map<Integer, Integer> yBoxMap;

        static {
            xBoxMap = new TreeMap<>();
            yBoxMap = new TreeMap<>();
            xBoxMap.put(0, 0); yBoxMap.put(0, 0);
            xBoxMap.put(1, 3); yBoxMap.put(1, 0);
            xBoxMap.put(2, 6); yBoxMap.put(2, 0);
            xBoxMap.put(3, 0); yBoxMap.put(3, 3);
            xBoxMap.put(4, 3); yBoxMap.put(4, 3);
            xBoxMap.put(5, 6); yBoxMap.put(5, 3);
            xBoxMap.put(6, 0); yBoxMap.put(6, 6);
            xBoxMap.put(7, 3); yBoxMap.put(7, 6);
            xBoxMap.put(8, 6); yBoxMap.put(8, 6);
        }

        public BoxTask(int[][] board, int box) {
            this.board = board;
            this.box = box;
        }

        @Override
        public Boolean call() {
            Set<Integer> vis = new TreeSet<>();
            int x = xBoxMap.get(box);
            int y = yBoxMap.get(box);

            for (int i = x; i < x + BOX_DIMENSION; i++) {
                for (int j = y; j < y + BOX_DIMENSION; j++) {
                    if (vis.contains(board[i][j])) {
                        return false;
                    } else {
                        vis.add(board[i][j]);
                    }
                }
            }
            return true;
        }
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        int[][] board = {
                {6, 2, 4, 5, 3, 9, 1, 8, 7},
                {5, 1, 9, 7, 2, 8, 6, 3, 4},
                {8, 3, 7, 6, 1, 4, 2, 9, 5},
                {1, 4, 3, 8, 6, 5, 7, 2, 9},
                {9, 5, 8, 2, 4, 7, 3, 6, 1},
                {7, 6, 2, 3, 9, 1, 4, 5, 8},
                {3, 7, 1, 9, 5, 6, 8, 4, 2},
                {4, 9, 6, 1, 8, 2, 5, 7, 3},
                {2, 8, 5, 4, 7, 3, 9, 1, 6},
        };

        ExecutorService pool = Executors.newCachedThreadPool();
        boolean valid_board = true;
        ArrayList<Future<Boolean>> results = new ArrayList<>();
        results.ensureCapacity(9 * 3);
        for (int i = 0; i < 9; i++) {
            Future<Boolean> row_i = pool.submit(new RowTask(board, i));
            Future<Boolean> col_i = pool.submit(new ColumnTask(board, i));
            Future<Boolean> box_i = pool.submit(new BoxTask(board, i));

            results.add(row_i);
            results.add(col_i);
            results.add(box_i);
        }

        for (Future<Boolean> b : results)
            valid_board &= b.get();

        System.out.println(valid_board);
        pool.shutdown();
    }
}
