#define FIXED_BIT 12

//This function is for converting a floating point
//number representated to fixed representation

unsigned short int float2fix(float n)
//unsigned long int float2fix(float n)
{
    unsigned short int int_part = 0, frac_part = 0;
  //  unsigned long int int_part = 0, frac_part = 0;
    int i;
    float t;

    int_part = (int)floor(n) << FIXED_BIT;
    n -= (int)floor(n);

    t = 0.5;
    for (i = 0; i < FIXED_BIT; i++) {
        if ((n - t) >= 0) {
            n -= t;
            frac_part += (1 << (FIXED_BIT - 1 - i));
        }
        t = t /2;
    }

    return int_part + frac_part;
}

//This function is for converting a fixed point
//number representated to floating representation

float fix2float(unsigned short int n)
//float fix2float(unsigned long int n)
{
    float ret = 0.0;
    int integer_part;
    int i;

    integer_part = (n >> FIXED_BIT);
    ret += integer_part;
    for (i = 0; i < FIXED_BIT; i++) {
        ret += ((n >> (FIXED_BIT - i - 1)) & (0x01)) * (1.0 / pow(2, i+1));
    }

    return ret;
}

