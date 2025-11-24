#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//ASM implementation
extern void imgCvtGrayFloatToInt(int height, int width, float* input, unsigned char* output);

//C implementation
void c_imgCvtGrayFloatToInt(int height, int width, float* input, unsigned char* output) {
    /* Note 6: You will need to check the correctness of your output.
    Formula: f/i = 1/255 -> i = f * 255
    */
    int i;
    int total_num_pixels = height * width;
    
    for(i = 0; i < total_num_pixels; i++) {
    output[i] = (unsigned char)(input[i] * 255.0f); 
    }
}

void displayInput(float* image, int height, int width) {
    int i, j;
    int index;

    printf("Input: Single float pixel values\n");

    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            index = i * width + j;

            printf("%.2f", image[index]); //print the integer

            if(j < width - 1) {
                printf(" "); //space after each value (e.g. 0.25 0.35 and so on)
            }
        }
        printf("\n"); //new line after each row
    }
    printf("\n");
}

void displayOutput(const char* label, unsigned char* image, int height, int width) {
    int i, j;
    int index;

    printf("Output (%s): Integer pixel values\n", label);

    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            index = i * width + j;

            printf("%3d", image[index]); //print the integer

            if(j < width - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void checkCorrectness(int total_num_pixels, float* input_image, unsigned char* output_image) {
    /* Note 6: You will need to check the correctness of your output.
    Formula: f/i = 1/255 -> i = f * 255
    */
   int count_correct = 0;
   unsigned char expected_output; //unsigned char -> because we need 8 bit unsigned integer
   int i;
   
   printf("Correctness Check: ");

   for(i = 0; i < total_num_pixels; i++) {
    expected_output = (unsigned char)(input_image[i] * 255.0f); 

    if(expected_output == output_image[i]) {
        count_correct++;
        }  
    }
     
    if(count_correct == total_num_pixels) {
        printf("Passed! All %d pixels match.\n", total_num_pixels);
    } else {
        printf("Failed. %d mismatches found.\n", total_num_pixels - count_correct);
    }    
}

int main() {
    int height, width;
    int i, j, k;
    int total_pixels_interactive;

    //INTERACTIVE
    printf("Enter image height: ");
    scanf("%d", &height);
    printf("Enter image width: ");
    scanf("%d", &width);

    total_pixels_interactive = width * height;

    /* Note 1: C is responsible for: allocating memory spaces for the images
        Reference: https://www.geeksforgeeks.org/c/memory-layout-of-c-program/
        Sample: ptr = (int*)malloc(n * sizeof(int));
        */
        float* input_image = (float*)malloc(total_pixels_interactive * sizeof(float)); //single float based pixel values between O to 1
        unsigned char* c_output_interactive = (unsigned char*)malloc(total_pixels_interactive * sizeof(unsigned char)); //8 bit unsigned integer. O for black, 255 for white, ...
        unsigned char* asm_output_interactive = (unsigned char*)malloc(total_pixels_interactive * sizeof(unsigned char)); 

        if(input_image == NULL || c_output_interactive == NULL || asm_output_interactive == NULL){
            printf("Memory Allocation Failed!\n");
            return 1;
        }

        srand((unsigned int)(time(NULL) ^ clock()));
        printf("Generating random values...\n");
        // Note 3: You may use a random number generator to generate pixel values for the input.
       for(i = 0; i < total_pixels_interactive; i++){
            input_image[i] = (float)rand() / (float)RAND_MAX; //Reference: https://www.geeksforgeeks.org/cpp/generate-a-random-float-number-in-cpp/
        }
        
        printf("\n");
        displayInput(input_image, height, width);

        //C OUTPUT
        c_imgCvtGrayFloatToInt(height, width, input_image, c_output_interactive); 
        displayOutput("C Implementation", c_output_interactive, height, width);

        //ASM OUTPUT
        imgCvtGrayFloatToInt(height, width, input_image, asm_output_interactive);
        displayOutput("Assembly Implementation", asm_output_interactive, height, width);

        //Reference: https://www.w3schools.com/c/c_memory_deallocate.php
       free(input_image);
       free(c_output_interactive);
       free(asm_output_interactive);
       
       int input_checker_sizes[] = {10, 100, 1000}; //{10*10, 100*100, and 1000*1000}
       int num_sizes = 3;

    /* Reference: https://www.w3schools.com/c/ref_stdlib_srand.php
    Syntax: srand(unsigned int seed);
    */
    srand((unsigned int)time(NULL)); 

    for(i = 0; i < num_sizes; i++){
        int checker_width = input_checker_sizes[i];
        int checker_height = input_checker_sizes[i];
        int total_pixels_checker = checker_width * checker_height;

        printf("----------------------------------------------------\n");
        printf("Image Size: %d x %d (Total: %d pixels)\n", checker_width, checker_height, total_pixels_checker);

        /* Note 1: C is responsible for: allocating memory spaces for the images
        Reference: https://www.geeksforgeeks.org/c/memory-layout-of-c-program/
        Sample: ptr = (int*)malloc(n * sizeof(int));
        */
        float* input_image_checker = (float*)malloc(total_pixels_checker * sizeof(float)); //single float based pixel values between O to 1
        unsigned char* c_output_checker = (unsigned char*)malloc(total_pixels_checker * sizeof(unsigned char)); //8 bit unsigned integer. O for black, 255 for white, ...
        unsigned char* asm_output_checker = (unsigned char*)malloc(total_pixels_checker * sizeof(unsigned char)); 

        if(input_image_checker == NULL || c_output_checker == NULL || asm_output_checker == NULL){
            printf("Memory Allocation Failed!\n");
            return 1;
        }

        // Note 3: You may use a random number generator to generate pixel values for the input.
       for(j = 0; j < total_pixels_checker; j++){
            input_image_checker[j] = (float)rand() / (float)RAND_MAX; //Reference: https://www.geeksforgeeks.org/cpp/generate-a-random-float-number-in-cpp/
        }

        /* Note 4: You must run at least 30 times to get the average execution time.
        Reference: https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9
        */
       clock_t start, end;
       double c_total_time = 0.0;
       double c_average_time = 0.0;
       double asm_total_time = 0.0;
       double asm_average_time = 0.0;
       int runs = 30;

       //C IMPLEMENTATION
       start = clock();
       for(k = 0; k < runs; k++){
        c_imgCvtGrayFloatToInt(checker_height, checker_width, input_image_checker, c_output_checker); 
       }
       end = clock();

       c_total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
       c_average_time = c_total_time / runs;

       printf("\nC Implementation\n");
       printf("Average Execution Time (%d runs): %f seconds\n", runs, c_average_time);

       checkCorrectness(total_pixels_checker, input_image_checker, c_output_checker);

       //ASM IMPLEMENTATION
       start = clock();
       for(k = 0; k < runs; k++){
        imgCvtGrayFloatToInt(checker_height, checker_width, input_image_checker, asm_output_checker); 
       }
       end = clock();

       asm_total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
       asm_average_time = asm_total_time / runs;

       printf("\nAssembly Implementation\n");
       printf("Average Execution Time (%d runs): %f seconds\n", runs, asm_average_time);

       checkCorrectness(total_pixels_checker, input_image_checker, asm_output_checker);

       //Reference: https://www.w3schools.com/c/c_memory_deallocate.php
       free(input_image_checker);
       free(c_output_checker);
       free(asm_output_checker);
    }
       printf("----------------------------------------------------\n");
       printf("Program finished.\n");
       
       return 0;
}
