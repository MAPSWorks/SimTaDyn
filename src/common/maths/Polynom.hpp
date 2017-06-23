#ifndef POLYNOM_HPP_
#  define POLYNOM_HPP_

#  include <vector>
#  include <iostream>

// *************************************************************************************************
//
// *************************************************************************************************
class Polynom
{
public:

  //! \brief Constructor with possibility to reserve memory for
  //! storing coefs.
  Polynom(const size_t nth = 0)
  {
    size_t i = nth;

    m_coefs.reserve(nth);
    while (i--)
      {
        m_coefs.push_back(0.0);
      }
  }

  //! \brief Return the degree of the polynom.
  inline uint32_t degree() const
  {
    return m_coefs.size();
  }

  //! \brief Change the value of the nth coef. If nth is greater than
  //! the current degree, intermediate coef are set to 0.  is greater
  //! than the current degree.
  inline double& operator[](size_t nth)
  {
    const uint32_t s = m_coefs.size();
    if (nth >= s)
      {
        // Insert zeros
        uint32_t i = nth - s + 1U;
        m_coefs.reserve(i);
        while (i--)
          {
            m_coefs.push_back(0.0);
          }
      }
    return m_coefs[nth];
  }

  //! \brief Read the nth coef. No security is trying to access to an
  //! coef with higher degree than the degree of the polynom.
  inline const double& operator[](size_t nth) const
  {
    return m_coefs[nth];
  }

  //! \brief Pretty print the polynom.
  void debug() const
  {
    std::cout << "Degree: " << degree() << std::endl;
    for (size_t i = 0; i < m_coefs.size(); ++i)
      {
        std::cout << "  P[" << i << "]: " << m_coefs[i] << std::endl;
      }
  }

protected:

  //! \brief Store coef values.
  std::vector<double> m_coefs;
};


#endif