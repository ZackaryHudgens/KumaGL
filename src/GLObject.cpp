#include "KumaGL/GLObject.hpp"

namespace KumaGL
{
    /******************************************************************************/
    GLObject::GLObject(GLObject &&aObject)
    {
        mID = aObject.mID;
        aObject.mID = 0;
    }

    /******************************************************************************/
    GLObject &GLObject::operator=(GLObject &&aObject)
    {
        mID = aObject.mID;
        aObject.mID = 0;

        return *this;
    }
} // namespace KumaGL
