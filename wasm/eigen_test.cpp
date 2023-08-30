#include "eigen_test.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

extern "C"
{
    EMSCRIPTEN_KEEPALIVE int simple_add(int a, int b)
    {
        // Eigen::Vector2d m;
        Eigen::MatrixXi m(2, 2);
        m(0, 0) = 3;
        m(1, 0) = 2;
        m(0, 1) = -1;
        m(1, 1) = 10;
        return m(1, 1);
    }

    EMSCRIPTEN_KEEPALIVE int test_malloc(int a, int b)
    {
        char *str;

        /* 最初的内存分配 */
        str = (char *)malloc(15);
        strcpy(str, "runoob");
        printf("String = %s,  Address = %u\n", str, str);

        /* 重新分配内存 */
        str = (char *)realloc(str, 25);
        strcat(str, ".com");
        printf("String = %s,  Address = %u\n", str, str);

        free(str);

        return 1;
    }
}