#pragma once

#ifndef L
#define L 384 // linear system size
#endif

#ifndef BLOCK
#define BLOCK L
#endif

#ifndef TEMP_INITIAL
#define TEMP_INITIAL 0.0 // initial temperature
#endif

#ifndef TEMP_FINAL
#define TEMP_FINAL 5.0 // final temperature
#endif

#ifndef TEMP_DELTA
#define TEMP_DELTA 0.01 // temperature step
#endif

#ifndef TRAN
#define TRAN 5 // equilibration time
#endif

#ifndef TMAX
#define TMAX 25 // measurement time
#endif

#ifndef DELTA_T
#define DELTA_T 5 // sampling period for energy and magnetization
#endif

#ifndef WIDTH
#define WIDTH (L/2)
#endif

#ifndef HEIGHT
#define HEIGHT (L)
#endif
