
public class Library {
	static {
		System.loadLibrary("library");
	}

	public native long[] first_70_fib_cpp();

	public long[] first_70_fib_java() {
		long[] res = new long[70];
		long before = 0;
		long before_before = 1;
		long fib = 0;
		for (int i = 0; i < 70; i++) {
			res[i] = fib;
			fib = before + before_before;
			before_before = before;
			before = fib;
		}
		return res;
	}

	public static void main(String[] args) {
		Library lib = new Library();
		double end;
		long start;

		double java_total = 0;
		double cpp_total = 0;

		for (int j = 0; j < 100; j++) {
			start = System.nanoTime();
			lib.first_70_fib_cpp();
			end = (double) (System.nanoTime() - start) / 1_000_000d;
			cpp_total += end;
			System.out.println("Cpp finished in " + end + "ms");

			start = System.nanoTime();
			lib.first_70_fib_java();
			end = (double) (System.nanoTime() - start) / 1_000_000d;
			java_total += end;
			System.out.println("Java finished in " + end + "ms");
		}
		System.out.println("Cpp avg time was " + (cpp_total / 100) + "ms\n" + "Java avg time was "
				+ (java_total / 100) + "ms");
	}
}
