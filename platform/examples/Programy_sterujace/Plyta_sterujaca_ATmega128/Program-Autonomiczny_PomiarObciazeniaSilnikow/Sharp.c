
#include <Sharp.h>
#include <math.h>
#include "Stepper2.h"
#include "Adc.h"

#define SHARP_USE_TABLE


volatile unsigned char sharp_dist_table[256]=
{
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
255,
114,
111,
108,
106,
104,
101,
98,
96,
94,
92,
90,
88,
86,
84,
82,
80,
78,
77,
75,
73,
71,
70,
68,
67,
65,
64,
63,
61,
60,
59,
57,
56,
55,
54,
53,
52,
51,
50,
49,
48,
48,
46,
46,
45,
44,
43,
42,
42,
41,
40,
40,
39,
39,
38,
38,
37,
36,
36,
36,
35,
35,
34,
34,
34,
33,
33,
32,
32,
32,
32,
31,
31,
30,
30,
30,
30,
30,
29,
29,
29,
28,
28,
28,
28,
28,
28,
27,
27,
27,
27,
26,
26,
26,
26,
26,
26,
26,
26,
25,
25,
25,
25,
24,
24,
24,
24,
24,
24,
24,
24,
23,
23,
23,
23,
22,
22,
22,
22,
22,
22,
22,
21,
21,
21,
21,
20,
20,
20,
20,
20,
20,
19,
19,
19,
18,
18,
18,
18,
18,
18,
17,
17,
17,
17,
17,
17,
16,
16,
16,
15,
15,
15,
15,
15,
15,
14,
14,
14,
13,
13,
13,
13,
13,
13,
13,
12,
12,
12,
11,
11,
11,
11,
11,
11,
11,
11,
11,
11,
11,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
11,
11,
11,
11,
11,
11,
11,
11,
11,
12,
12,
13,
13,
13,
13,
14,
15,
15,
15,
16,
17,
17,
17,
18,
18,
19,
20,
21,
22,
22,
24,
24,
26,
26,
28,
28,
30,
31,
};



unsigned int sharp_scan(unsigned int adc_val)
{

    #ifdef SHARP_USE_TABLE
        
        unsigned char pomiar;
        unsigned int wynik;
        pomiar = (unsigned char)(adc_val>>2);
        wynik = *(sharp_dist_table + pomiar); 
        
    #else

        #define p1 -11.395*10.0
        #define p2 105.11*10.0
        #define p3 -385.15*10.0
        #define p4 710.25*10.0
        #define p5 -690.56*10.0
        #define p6 333.08*10.0
        
        float pomiar;
        float wynik;
        
        pomiar=adc_val*conv_mul;
        wynik=(p1*pow(pomiar,5)+p2*pow(pomiar,4)+p3*pow(pomiar,3)+p4*pow(pomiar,2)+p5*pomiar+p6);  
    #endif

    global_sharp_dist = (unsigned int)(wynik);
    
    return (unsigned int)(wynik);
}
