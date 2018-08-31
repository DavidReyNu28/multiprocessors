/* This code implements the known sort algorithm "Counting sort" */
import java.util.Arrays;

public class MainExample3 {
	private static int SIZE = 10_000;
	private static final int MAXTHREADS = Runtime.getRuntime().availableProcessors();
	
	public static void main(String args[]) {
		Example3 threads[];
		int block;
		long startTime, stopTime;
		double acum = 0;
		
		int array[] = new int[SIZE];
		int temp[] = new int[SIZE];
		Utils.randomArray(array);
		Utils.displayArray("before", array);
		
		block = SIZE / MAXTHREADS;
		threads = new Example3[MAXTHREADS];
		
		acum = 0;
		for (int j = 1; j <= Utils.N; j++) {
			for (int i = 0; i < threads.length; i++) {
				if (i != threads.length - 1) {
					threads[i] = new Example3(array, temp, (i * block), ((i + 1) * block));
				} else {
					threads[i] = new Example3(array, temp, (i * block), SIZE);
				}
			}
			
			startTime = System.currentTimeMillis();
			for (int i = 0; i < threads.length; i++) {
				threads[i].start();
			}
			for (int i = 0; i < threads.length; i++) {
				try {
					threads[i].join();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			stopTime = System.currentTimeMillis();
			acum +=  (stopTime - startTime);
		}
		
		array = Arrays.copyOf(temp, temp.length);
		
		Utils.displayArray("after", array);
		System.out.printf("avg time = %.5f\n", (acum / Utils.N));
	}
}
			
