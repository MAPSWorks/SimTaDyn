#ifndef CONE_HPP
#define CONE_HPP

#  include "MeshQQ.hpp"

// Inspiration from:
// http://www.freemancw.com/2012/06/opengl-cone-function/

class MeshCone: public MeshQQ
{
public:

  MeshCone(Vector3f const &d, Vector3f const &a,
           const float h, const float rd, const uint8_t n)
    : MeshQQ()
  {
    const float fn = n;
    const float M_PI_DIV180 = 0.01745329251f;
    Vector3f c = a + (-d * h);
    Vector3f e0 = perp(d);
    Vector3f e1 = vector::cross(e0, d);
    float angInc = 360.0f / fn * M_PI_DIV180;

    m_pos.push_back(a.x);
    m_pos.push_back(a.y);
    m_pos.push_back(a.z);

    m_tex.push_back(0.5f);
    m_tex.push_back(0.0f);

    // calculate points around directrix
    for (uint8_t i = 0U; i < n; ++i)
      {
        float rad = angInc * i;
        Vector3f p = c + (((e0 * std::cos(rad)) + (e1 * std::sin(rad))) * rd);
        m_pos.push_back(p.x);
        m_pos.push_back(p.y);
        m_pos.push_back(p.z);

        if (i & 1) {
          m_tex.push_back(1.0f);
          m_tex.push_back(1.0f);
        } else {
          m_tex.push_back(0.0f);
          m_tex.push_back(1.0f);
        }
    }

    /* m_pos[Bottom].push_back(c.x);
    m_pos[Bottom].push_back(c.y);
    m_pos[Bottom].push_back(c.z);

    // draw cone bottom
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(c.x(), c.y(), c.z());
    for (int i = n-1; i >= 0; --i) {
        glVertex3f(pts[i].x(), pts[i].y(), pts[i].z());
    }
    glEnd();*/
  }

  inline void draw() { MeshQQ::draw(GL_TRIANGLE_FAN); }

private:

  Vector3f perp(Vector3f const &v)
  {
    float min = maths::abs(v.x);
    Vector3f cardinalAxis(1.0f, 0.0f, 0.0f);

    float y = maths::abs(v.y);
    if (y < min)
      {
        min = y;
        cardinalAxis = Vector3f(0.0f, 1.0f, 0.0f);
      }

    if (maths::abs(v.z) < min)
      {
        cardinalAxis = Vector3f(0.0f, 0.0f, 1.0f);
      }

    return vector::cross(v, cardinalAxis);
  }
};

#endif
