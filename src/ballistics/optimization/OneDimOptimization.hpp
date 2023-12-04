#ifndef PROJECT_BALLISTICS_ONEDIMOPTIMIZATION_HPP
#define PROJECT_BALLISTICS_ONEDIMOPTIMIZATION_HPP

#include "ballistics/types/BasicTypes.hpp"

namespace Ballistics::Optimization::Dichotomy
{
    template<typename FuncType, typename Stopper>
    [[nodiscard]] typename FuncType::ArgType Optimize(const typename FuncType::ArgType lBorder,
                                                      const typename FuncType::ArgType rBorder) {
        typename FuncType::ArgType aTmp = lBorder;
        typename FuncType::ArgType bTmp = rBorder;
        typename FuncType::ArgType cTmp = (aTmp + bTmp) / 2.;

        while (!Stopper::isFinish(aTmp, bTmp)) {
            const typename FuncType::ArgType funcValInC = FuncType::calcFunc(cTmp);
            const typename FuncType::ArgType yTmp = (cTmp + aTmp) / 2.;

            if (FuncType::calcFunc(yTmp) <= funcValInC) {
                bTmp = cTmp;
                cTmp = yTmp;
            } else {
                const typename FuncType::ArgType zTmp = (cTmp + bTmp) / 2.;

                if (funcValInC <= FuncType::calcFunc(zTmp)) {
                    aTmp = yTmp;
                    bTmp = zTmp;
                } else {
                    aTmp = cTmp;
                    cTmp = zTmp;
                }
            }
        }
        return cTmp;
    }
}

#endif //PROJECT_BALLISTICS_ONEDIMOPTIMIZATION_HPP
