#ifndef SHAPECREATOR_HPP
#define SHAPECREATOR_HPP

#include <map>
#include "shapetypes.hpp" // Burada enum class ShapeTypes tanımlı

namespace graf {

    class VertexArrayObject;

    class ShapeCreator {
    public:
        static VertexArrayObject* createShape(ShapeTypes shape);

    private:
        ShapeCreator() {}
        static ShapeCreator* m_Instance;

        std::map<ShapeTypes, VertexArrayObject*> m_vaoMap;

        static VertexArrayObject* createSquare();
        static VertexArrayObject* createCircle(int anglesInDegrees=10);
        static VertexArrayObject* createCube();
        static VertexArrayObject* createPyramid();
        static VertexArrayObject* createCutPyramid();
    };

} // namespace graf

#endif
