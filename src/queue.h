/*
 * 	TODO: If you have any imports to make, place them here.
 */

import java.util.Scanner;

// can import java.lang.Math

public class ArrayPractice {
	/*
	 * 	TODO: Supply the needed methods here.
	 */

	public double[] getNumbers() {
		// note: NEED TO MAKE SCANNER OBJECT
		Scanner scanner = new Scanner(System.in);

		int n = scanner.nextInt();

		System.out.println("--------");

		// array instantiation:
		double[] numbers = new double[n];

        for (int i = 0; i < n; i++) {
            numbers[i] = scanner.nextDouble();
        }

        System.out.println("--------");

        // CLOSE SCANNER
        scanner.close();

        return numbers;
	}

	public double computeAvg(double[] numbers) {
		double sum = 0.0;
		for (double num : numbers) {
			sum += num;
		}
		return sum / numbers.length;
	}

	public double computeStd(double[] numbers) {
		double average = computeAvg(numbers);
		double sumOfSquares = 0.0;
		for (double num : numbers) {
			sumOfSquares += Math.pow(num - average, 2);
		}
		return Math.sqrt(sumOfSquares / numbers.length);
	}

	/*
	 * 	Do NOT modify the main method.
	 */
	public static void main(String[] args) {
		ArrayPractice ap = new ArrayPractice();

		double[] numbers = ap.getNumbers();

		System.out.println("Avg: " + ap.computeAvg(numbers));
		System.out.println("Std: " + ap.computeStd(numbers));
	}
}
