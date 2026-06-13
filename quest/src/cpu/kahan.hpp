/** @file
 * @brief Neumaier-compensated summation for numerically stable
 *        accumulation in dense matrix application.
 *
 * @author [Your Name]
 */

#ifndef KAHAN_HPP
#define KAHAN_HPP

#include "quest/include/types.h"
#include <cmath>
#include <complex>

// Neumaier's improved Kahan-Babuska summation (handles |x| > |sum| case
// that classic Kahan misses). Reduces accumulated rounding error from
// O(n *eps) to O(eps), independent of the number of terms summed.
struct NeumaierAcc {
    qreal sum  = 0.0;
    qreal comp = 0.0;

    inline void add(qreal x) {
        qreal t = sum + x;
        if (std::abs(sum) >= std::abs(x))
            comp += (sum - t) + x;
        else
            comp += (x - t) + sum;
        sum = t;
    }

    inline qreal result() const { return sum + comp; }
};

struct NeumaierAccComplex {
    NeumaierAcc re, im;

    inline void add(qcomp val) {
        re.add(std::real(val));
        im.add(std::imag(val));
    }

    inline qcomp result() const {
        return qcomp(re.result(), im.result());
    }
};

#endif // KAHAN_HPP