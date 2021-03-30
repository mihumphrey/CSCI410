class c {
    static void Main() {
        int[] arr = new int[10];
        for (int i = 0; i < 10; i++) {
            int input = 0;
            while (input < 10) {
                Console.WriteLine("Enter value " + i);
                int input = Convert.ToInt32(Console.ReadLine());
                if (input < 10)
                    Console.WriteLine("Invalid input");
            }
            arr[i] = input; 
        }

        for (int i = 0; i < 10; i++) {
            // Find the minimum element in unsorted array 
            int min_idx = i; 
            for (int j = i + 1; j < 10; j++) 
                if (arr[j] < arr[min_idx]) 
                    min_idx = j; 
            int temp = arr[min_idx]; 
            arr[min_idx] = arr[i]; 
            arr[i] = temp; 
        } 

        for (int i = 0; i < 10; i++) {
            Console.WriteLine(arr[i]);
        }
    }
}
