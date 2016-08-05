#include "globject.hpp"

namespace wheels {
namespace opengl {
void object::swap(object &o) { std::swap(glfun, o.glfun); }
}
}