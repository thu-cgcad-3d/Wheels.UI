/* * *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Hao Yang (yangh2007@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * * */

#pragma once

#include "globject.hpp"

namespace wheels {
namespace opengl {
class buffers : public object {
public:
  enum class type : GLenum {
    // The buffer will be used as a source for vertex data, but the connection
    // is only made when glVertexAttribPointer is called. The pointer field of
    // this function is taken as a byte offset from the beginning of whatever
    // buffer is currently bound to this target.
    array = GL_ARRAY_BUFFER,
    // All rendering functions of the form gl*Draw*Elements* will use the
    // pointer field as a byte offset from the beginning of the buffer object
    // bound to this target. The indices used for indexed rendering will be
    // taken from the buffer object. Note that this binding target is part of a
    // Vertex Array Objects state, so a VAO must be bound before binding a
    // buffer here.
    element_array = GL_ELEMENT_ARRAY_BUFFER,
    // These have no particular semantics. Because they have no actual meaning,
    // they are useful targets for copying buffer object data with
    // glCopyBufferSubData. You do not have to use these targets when copying,
    // but by using them, you avoid disturbing buffer targets that have actual
    // semantics.
    copy_read = GL_COPY_READ_BUFFER,
    copy_write = GL_COPY_WRITE_BUFFER,
    // These are for performing asynchronous pixel transfer operations. If a
    // buffer is bound to GL_PIXEL_UNPACK_BUFFER, glTexImage*, glTexSubImage*,
    // glCompressedTexImage*, and glCompressedTexSubImage* are all affected.
    // These functions will read their data from the bound buffer object instead
    // of where a client pointer points. Similarly, if a buffer is bound to
    // GL_PIXEL_PACK_BUFFER, glGetTexImage, and glReadPixels will store their
    // data to the bound buffer object instead of where a client pointer points.
    pixel_unpack = GL_PIXEL_UNPACK_BUFFER,
    pixel_pack = GL_PIXEL_PACK_BUFFER,
    // These are for performing direct writes from asynchronous queries to
    // buffer object memory. If a buffer is bound to GL_QUERY_BUFFER then all
    // glGetQueryObject[ui64v] function calls will write the result to an offset
    // into the bound buffer object.
    query = GL_QUERY_BUFFER,
    // This target has no special semantics, but if you intend to use a buffer
    // object for Buffer Textures, it is a good idea to bind it here when you
    // first create it.
    texture = GL_TEXTURE_BUFFER,
    // An indexed buffer binding for buffers used in Transform Feedback
    // operations.
    transform_feedback = GL_TRANSFORM_FEEDBACK_BUFFER,
    // An indexed buffer binding for buffers used as storage for uniform blocks.
    uniform = GL_UNIFORM_BUFFER,
    // The buffer bound to this target will be used as the source for the
    // indirect data when performing indirect rendering. This is only available
    // in core OpenGL 4.0 or with ARB_draw_indirec
    draw_indirect = GL_DRAW_INDIRECT_BUFFER,
    // An indexed buffer binding for buffers used as storage for atomic
    // counters. This requires OpenGL 4.2 or ARB_shader_atomic_counters
    atomic_counter = GL_ATOMIC_COUNTER_BUFFER,
    // The buffer bound to this target will be used as the source for indirect
    // compute dispatch operations, via glDispatchComputeIndirect. This
    // requires OpenGL 4.3 or ARB_compute_shader
    dispatch_indirect = GL_DISPATCH_INDIRECT_BUFFER,
    // An indexed buffer binding for buffers used as storage for shader storage
    // blocks. This requires OpenGL 4.3 or ARB_shader_storage_buffer_object
    shader_storage = GL_SHADER_STORAGE_BUFFER,
  };

  enum class data_usage : GLenum {
    stream_draw = GL_STREAM_DRAW,
    stream_read = GL_STREAM_READ,
    stream_copy = GL_STREAM_COPY,
    static_draw = GL_STATIC_DRAW,
    static_read = GL_STATIC_READ,
    static_copy = GL_STATIC_COPY,
    dynamic_draw = GL_DYNAMIC_DRAW,
    dynamic_read = GL_DYNAMIC_READ,
    dynamic_copy = GL_DYNAMIC_COPY,
  };

public:
  explicit buffers(glfunctions *fun, size_t n);
  explicit buffers(glfunctions *fun, GLuint *fbs, size_t n);
  explicit buffers(glfunctions *fun, type t, void *ptr, size_t nbytes,
                   data_usage usage);
  virtual ~buffers();

  buffers(const buffers &) = delete;
  buffers(buffers &&fb);
  buffers &operator=(const buffers &) = delete;
  buffers &operator=(buffers &&fb);

  void swap(buffers &fb);
  size_t size() const { return _size; }

  buffers operator[](size_t i) const;

public:
  void set_data(void *ptr, size_t nbytes, data_usage usage, size_t i = 0) const;
  uint8_t const *ptr(size_t i = 0) const;
  size_t nbytes(size_t i = 0) const;
  data_usage usage(size_t i = 0) const;

private:
  type _type;
  size_t _size;
  GLuint *_buffers;
  bool _own_res;
};
}
}