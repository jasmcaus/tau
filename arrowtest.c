#include <stdio.h>
#include <Arrow/Types.h>

int main() {
    // Max number of digits of a UInt64 number is 19
    UInt64 n; 
    double num = 37;
    int n_digits = 0; 
    n = (UInt64)num;
    while(n!=0) {
        n/=10;
        ++n_digits;
    }
    
    // Stick with nanoseconds (no need for decimal points here)
    if(n_digits < 3) 
        printf("%.0lfns\n", num);

    else if(n_digits >= 3 && n_digits < 6)
        printf("%.2lfµs\n", num/1000);
        
    else if(n_digits >= 6 && n_digits <= 9)
        printf("%.2lfms\n", num/1000000);

    else
        printf("%.2lfs\n", num/1000000000);
}