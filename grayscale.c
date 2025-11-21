#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void imgCvtGrayFloatToInt(int height, int width, float* input, unsigned char* output); //MIGHT CHANGE

void checkCorrectness(int total_num_pixels, float* input_image, unsigned char* output_image) {
    /* Note 6: You will need to check the correctness of your output.
    Formula: f/i = 1/255 -> i = f * 255
    */
   int count_correct = 0;
   unsigned char expected_output; //unsigned char -> because we need 8 bit unsigned integer
   int i;
   
   for(i = 0; i < total_num_pixels; i++) {
    expected_output = (unsigned char)(input_image[i] * 255.0f); //MIGHT CHANGE (WAIT FOR ASSEMBLY)
    
    if(expected_output == output_image[i]) {
        count_correct++;
    }
        
    //Take a screenshot of the program output with the correctness check
    if(i < 10) {
        printf("%.4f \t -> \t %d \t vs \t %d\n", input_image[i], expected_output, output_image[i]); //MIGHT CHANGE
        }    
    }
     
    if(count_correct == total_num_pixels) {
        printf("\nCorrectness Check Passed! All %d pixels match.\n", total_num_pixels);
    } else {
        printf("\nCorrectness Check Failed. %d mismatches found.\n", total_num_pixels - count_correct);
    }    
}

int main() {
    int input_image_sizes[] = {10, 100, 1000}; //{10*10, 100*100, and 1000*1000}
    int num_sizes = 3;
    int i, j, k;

    /* Reference: https://www.w3schools.com/c/ref_stdlib_srand.php
    Syntax: srand(unsigned int seed);
    */
    srand((unsigned int)time(NULL)); 

    for(i = 0; i < num_sizes; i++){
        int width = input_image_sizes[i];
        int height = input_image_sizes[i];
        int total_num_pixels = width * height;

        printf("Processing Image Size: %d x %d (Total: %d pixels)\n", width, height, total_num_pixels);

        /* Note 1: C is responsible for: allocating memory spaces for the images
        Reference: https://www.geeksforgeeks.org/c/memory-layout-of-c-program/
        Sample: ptr = (int*)malloc(n * sizeof(int));
        */
        float* input_image = (float*)malloc(total_num_pixels * sizeof(float)); //single float based pixel values between O to 1
        unsigned char* output_image = (unsigned char*)malloc(total_num_pixels * sizeof(unsigned char)); //8 bit unsigned integer. O for black, 255 for white, ...

        if(input_image == NULL || output_image == NULL){
            printf("Memory Allocation Failed!\n");
            return 1;
        }

        // Note 3: You may use a random number generator to generate pixel values for the input.
       for(j = 0; j < total_num_pixels; j++){
            input_image[j] = (float)rand() / (float)RAND_MAX; //Reference: https://www.geeksforgeeks.org/cpp/generate-a-random-float-number-in-cpp/
        }

        /* Note 4: You must run at least 30 times to get the average execution time.
        Reference: https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9
        */
       clock_t start, end;
       double total_time = 0.0;
       double average_time = 0.0;
       int runs = 30;

       start = clock();
       for(k = 0; k < runs; k++){
        imgCvtGrayFloatToInt(height, width, input_image, output_image); //MIGHT CHANGE
       }
       end = clock();

       total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
       average_time = total_time / runs;

       printf("Average execution time given %d runs: %f seconds\n", runs, average_time);

       checkCorrectness(total_num_pixels, input_image, output_image);

       //Reference: https://www.w3schools.com/c/c_memory_deallocate.php
       free(input_image);
       free(output_image);
    }

    return 0;
}