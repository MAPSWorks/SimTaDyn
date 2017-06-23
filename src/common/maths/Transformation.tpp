// -*- c++ -*- Coloration Syntaxique pour Emacs
#ifndef TRANSFORMATION_TPP_
#  define TRANSFORMATION_TPP_

// Modifed from the OpenGL Mathematic library (GLM).
// Original code: https://github.com/g-truc/glm/blob/master/glm/gtc/matrix_transform.inl

#  include "Matrix.tpp"
#  include <cassert>

namespace matrix
{
  template<typename T>
  Matrix<T, 4U, 4U> translate(Matrix<T, 4U, 4U> const &m, Vector<T, 3U> const &v)
  {
    Matrix<T, 4U, 4U> M(m);

    M[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];

    return M;
  }

  template<typename T>
  Matrix<T, 4U, 4U> scale(Matrix<T, 4U, 4U> const &m, Vector<T, 3U> const &v)
  {
    Matrix<T, 4U, 4U> M;

    M[0] = m[0] * v[0];
    M[1] = m[1] * v[1];
    M[2] = m[2] * v[2];
    M[3] = m[3];

    return M;
  }

  template<typename T>
  Matrix<T, 4U, 4U> rotate(Matrix<T, 4U, 4U> const &m, T const angle, Vector<T, 3U> const &v)
  {
    T const a = angle;
    T const c = std::cos(a);
    T const s = std::sin(a);

    Vector<T, 3U> axis(vector::normalize(v));
    Vector<T, 3U> temp((T(1) - c) * axis);
    Matrix<T, 4U, 4U> rotate;

    rotate[0][0] = c + temp[0] * axis[0];
    rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    rotate[1][1] = c + temp[1] * axis[1];
    rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    rotate[2][2] = c + temp[2] * axis[2];

    Matrix<T, 4U, 4U> M;
    M[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
    M[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
    M[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
    M[3] = m[3];

    return M;
  }

  template<typename T>
  Matrix<T, 4U, 4U> ortho(T const left, T const right, T const bottom, T const top)
  {
    Matrix<T, 4U, 4U> M(matrix::Identity);

    M[0][0] = T(2) / (right - left);
    M[1][1] = T(2) / (top - bottom);
    M[2][2] = T(-1);
    M[3][0] = -(right + left) / (right - left);
    M[3][1] = -(top + bottom) / (top - bottom);

    return M;
  }

  // *************************************************************************************************
  //! \brief Replace gluPerspective(). Set the frustum to perspective mode.
  //! \param fovY     - Field of vision in degrees in the y direction.
  //! \param aspect   - Aspect ratio of the viewport.
  //! \param zNear    - The near clipping distance.
  //! \param zFar     - The far clipping distance.
  // *************************************************************************************************
  template<typename T>
  Matrix<T, 4U, 4U> perspective(T const fovy, T const aspect, T const zNear, T const zFar)
  {
    assert(std::abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

    T const tanHalfFovy = std::tan(fovy / T(2));
    Matrix<T, 4U, 4U> M(0);

    M[0][0] = T(1) / (aspect * tanHalfFovy);
    M[1][1] = T(1) / (tanHalfFovy);
    M[2][3] = T(-1);
    M[2][2] = - (zFar + zNear) / (zFar - zNear);
    M[3][2] = - (T(2) * zFar * zNear) / (zFar - zNear);

    return M;
}

  template<typename T>
  Matrix<T, 4U, 4U> lookAt(Vector<T, 3U> const &eye,
                           Vector<T, 3U> const &center,
                           Vector<T, 3U> const &up)
  {
    Vector<T, 3U> const f(vector::normalize(center - eye));
    Vector<T, 3U> const s(vector::normalize(vector::cross(f, up)));
    Vector<T, 3U> const u(vector::cross(s, f));
    Matrix<T, 4U, 4U> M(matrix::Identity);

    M[0][0] = s.x;
    M[1][0] = s.y;
    M[2][0] = s.z;
    M[0][1] = u.x;
    M[1][1] = u.y;
    M[2][1] = u.z;
    M[0][2] = -f.x;
    M[1][2] = -f.y;
    M[2][2] = -f.z;
    M[3][0] = -(vector::dot(s, eye));
    M[3][1] = -(vector::dot(u, eye));
    M[3][2] = vector::dot(f, eye);

    return M;
  }
}

#endif /* TRANSFORMATION_TPP_ */