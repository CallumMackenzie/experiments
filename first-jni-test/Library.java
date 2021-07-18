
// javac -h . Library.java
public class Library {
	static {
		System.loadLibrary("library");
	}

	private float x = 0;
	public native float getX();

	public native long[] first_fib_cpp(int nums);

	public long[] first_fib_java(int nums) {
		long[] res = new long[nums];
		for (int i = 0; i < nums; i++) {
			if (i == 0)
				res[i] = 0;
			else if (i == 1)
				res[i] = 1;
			else
				res[i] = res[i - 1] + res[i - 2];
		}
		return res;
	}

	public static void main(String[] args) {
		Library lib = new Library();
		double end;
		long start;

		double java_total = 0;
		double cpp_total = 0;
		int numGenerate = 100;

		for (int j = 0; j < 100; j++) {
			start = System.nanoTime();
			lib.first_fib_cpp(numGenerate);
			end = (double) (System.nanoTime() - start) / 1_000_000d;
			cpp_total += end;

			start = System.nanoTime();
			lib.first_fib_java(numGenerate);
			end = (double) (System.nanoTime() - start) / 1_000_000d;
			java_total += end;
		}
		System.out.println("Cpp avg time was " + (cpp_total / 100) + "ms\n" + "Java avg time was "
				+ (java_total / 100) + "ms");
	}
}
