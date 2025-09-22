#ifndef KUMAGL_HPP
#define KUMAGL_HPP

#include "glad/gl.h"

namespace KumaGL
{
    /******************************************************************************/
    inline bool InitKumaGL(GLADloadfunc aFunc)
    {
        bool success = true;
        if (gladLoadGL(aFunc) == 0)
        {
            success = false;
        }

        return success;
    }
} // namespace KumaGL

#endif // !KUMAGL_HPP
