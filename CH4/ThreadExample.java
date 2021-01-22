import java.util.concurrent.*;

class Summation implements Callable<Integer>
{
	private int upper;
	public Summation(int up) {
		this.upper = up;
	}

	public Integer call()
	{
		int sum = 0;
		for (int i = 1; i < this.upper + 1; i++)
			sum += i;
		return new Integer(sum);
	}
}

class ThreadExample {

	public static void main(String[] args)
	{
		ExecutorService pool = Executors.newSingleThreadExecutor();
		Future<Integer> futureRes = pool.submit(new Summation(Integer.parseInt(args[0])));

		try {
			System.out.println("Sum is: " + futureRes.get());
		} catch(InterruptedException | ExecutionException e) {
		}
	}

}
