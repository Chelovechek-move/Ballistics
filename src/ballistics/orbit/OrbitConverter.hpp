#ifndef PROJECT_BALLISTICS_ORBITCONVERTER_HPP
#define PROJECT_BALLISTICS_ORBITCONVERTER_HPP

#include "ballistics/types/BasicTypes.hpp"
#include "ballistics/orbit/CartesianElements.hpp"
#include "ballistics/orbit/KeplerMeanElements.hpp"
#include "ballistics/orbit/KeplerTrueElements.hpp"

namespace Ballistics::Orbit
{
    template<typename Tto, typename Tfrom>
    [[nodiscard]] Tto convert(const Tfrom &initialSetOfElements,
                              const scalar gravityParameter);

////////////////////////////////////CONVERSION FROM CARTESIAN TO KEPLERIAN//////////////////////////////////////////////
    template<>
    inline KeplerTrue convert<KeplerTrue, PosVel>(const PosVel &initSetOfElements,
                                                  const scalar gravityParameter)
    {
        const vector3d orbMoment = initSetOfElements.position.cross(initSetOfElements.velocity);
        const scalar orbMomentNorm = orbMoment.norm();
        const scalar inclination = atan2(sqrt(orbMoment.x() * orbMoment.x() +
                                              orbMoment.y() * orbMoment.y()), orbMoment.z());
        const vector3d x = {1., 0., 0.};
        const vector3d y = {0., 1., 0.};
        const vector3d z = {0., 0., 1.};

        const vector3d zVecMultByOrbMoment = z.cross(orbMoment);
        const scalar zVecMultByOrbMomentNorm = zVecMultByOrbMoment.norm();
        const vector3d dirToAscendingNode = (zVecMultByOrbMomentNorm != 0.) ? zVecMultByOrbMoment /
                                                                              zVecMultByOrbMomentNorm : x;

        const scalar longitudeOfAscendingNodeTmp = atan2(dirToAscendingNode.y(), dirToAscendingNode.x()) + 2 * M_PI;
        const scalar longitudeOfAscendingNode = (longitudeOfAscendingNodeTmp > 2 * M_PI) ? longitudeOfAscendingNodeTmp - 2 * M_PI :
                                                 longitudeOfAscendingNodeTmp;
        const scalar velocitySquared = initSetOfElements.velocity.dot(initSetOfElements.velocity);
        const scalar positionNorm = initSetOfElements.position.norm();
        const vector3d eccentricityVec = (1 / gravityParameter) * ((velocitySquared - gravityParameter / positionNorm) *
                initSetOfElements.position - initSetOfElements.position.dot(initSetOfElements.velocity) *
                initSetOfElements.velocity);

        const scalar eccentricity = eccentricityVec.norm();
        const vector3d eccentricity1Vec = (eccentricity == 0.) ? dirToAscendingNode : eccentricityVec / eccentricity;
        const vector3d orbitalMomentNormalized = orbMoment / orbMomentNorm;
        const scalar pericenterArgumentTmp = atan2(orbitalMomentNormalized.cross(dirToAscendingNode).dot(eccentricity1Vec),
                                                dirToAscendingNode.dot(eccentricity1Vec)) + 2 * M_PI;
        const scalar pericenterArgument = (pericenterArgumentTmp > 2 * M_PI) ? pericenterArgumentTmp - 2 * M_PI :
                                          pericenterArgumentTmp;
        const scalar largeSemiAxis = gravityParameter / ((2 * gravityParameter / positionNorm) - velocitySquared);
        const vector3d eccentricity2Vec = orbitalMomentNormalized.cross(eccentricity1Vec);

        const scalar trueAnomalyTmp = atan2(eccentricity2Vec.dot(initSetOfElements.position),
                                            eccentricity1Vec.dot(initSetOfElements.position)) + 2 * M_PI;
        const scalar trueAnomaly = (trueAnomalyTmp > 2 * M_PI) ? trueAnomalyTmp - 2 * M_PI : trueAnomalyTmp;

        return {inclination,
                longitudeOfAscendingNode,
                largeSemiAxis,
                eccentricity,
                pericenterArgument,
                trueAnomaly};
    }

    template<>
    inline KeplerMean convert<KeplerMean, PosVel>(const PosVel &initSetOfElements,
                                                  const scalar gravityParameter)
    {
        const KeplerTrue KeplerTrueElements = convert<KeplerTrue>(initSetOfElements, gravityParameter);
        const scalar sqrtOneMinusEccentricitySquared = sqrt(1. - KeplerTrueElements.eccentricity *
                                                                KeplerTrueElements.eccentricity);

        const scalar cosOfTrueAnomaly = cos(KeplerTrueElements.trueAnomaly);
        const scalar sinOfEccentricAnomaly = (sin(KeplerTrueElements.trueAnomaly) * sqrtOneMinusEccentricitySquared) /
                                             (1. + KeplerTrueElements.eccentricity * cosOfTrueAnomaly);
        const scalar eccentricAnomaly = atan2(sinOfEccentricAnomaly, (KeplerTrueElements.eccentricity + cosOfTrueAnomaly) /
                                                                     (1. + KeplerTrueElements.eccentricity * cosOfTrueAnomaly));
        const scalar meanAnomaly = eccentricAnomaly - KeplerTrueElements.eccentricity * sinOfEccentricAnomaly;

        return {KeplerTrueElements.inclination,
                KeplerTrueElements.longitudeOfAscendingNode,
                KeplerTrueElements.largeSemiAxis,
                KeplerTrueElements.eccentricity,
                KeplerTrueElements.pericenterArgument,
                meanAnomaly};
    }

////////////////////////////////////CONVERSION FROM CARTESIAN TO CARTESIAN//////////////////////////////////////////////
    template<>
    inline PosVel convert<PosVel, PosVel>(const PosVel &initSetOfElements,
                                          const scalar gravityParameter)
    {
        return initSetOfElements;
    }

////////////////////////////////////CONVERSION FROM KEPLERIAN TO KEPLERIAN//////////////////////////////////////////////
    template<>
    inline KeplerTrue convert<KeplerTrue, KeplerMean>(const KeplerMean &initSetOfElements,
                                                      const scalar gravParam)
    {
        // To convert mean anomaly to an eccentric anomaly, we will use Newton's method
        scalar eccentricAnomaly = initSetOfElements.meanAnomaly - initSetOfElements.eccentricity;
        for (integer i = 0; i < 4; ++i)
        {
            eccentricAnomaly = eccentricAnomaly - (eccentricAnomaly - initSetOfElements.eccentricity *
                                                   sin(eccentricAnomaly) - initSetOfElements.meanAnomaly) /
                                                  (1. - initSetOfElements.eccentricity * cos(eccentricAnomaly));
        }

        const scalar cosEccentricAnomaly = cos(eccentricAnomaly);
        const scalar sinTrueAnomaly = (sin(eccentricAnomaly) *
                                       sqrt(1. - initSetOfElements.eccentricity * initSetOfElements.eccentricity)) /
                                      (1. - initSetOfElements.eccentricity * cosEccentricAnomaly);
        const scalar cosTrueAnomaly = (cosEccentricAnomaly - initSetOfElements.eccentricity) /
                                      (1. - initSetOfElements.eccentricity * cosEccentricAnomaly);
        const scalar trueAnomaly = atan2(sinTrueAnomaly, cosTrueAnomaly);

        return {initSetOfElements.inclination,
                initSetOfElements.longitudeOfAscendingNode,
                initSetOfElements.largeSemiAxis,
                initSetOfElements.eccentricity,
                initSetOfElements.pericenterArgument,
                trueAnomaly};
    }

    template<>
    inline KeplerMean convert<KeplerMean, KeplerTrue>(const KeplerTrue &initSetOfElements,
                                                      const scalar gravParam)
    {
        const scalar cosTrueAnomaly = cos(initSetOfElements.trueAnomaly);
        const scalar sinEccentricAnomaly = (sin(initSetOfElements.trueAnomaly) *
                                            sqrt(1. - initSetOfElements.eccentricity * initSetOfElements.eccentricity)) /
                                           (1. + initSetOfElements.eccentricity * cosTrueAnomaly);
        const scalar cosEccentricAnomaly = (initSetOfElements.eccentricity + cosTrueAnomaly) /
                                           (1. + initSetOfElements.eccentricity * cosTrueAnomaly);
        const scalar eccentricAnomaly = atan2(sinEccentricAnomaly, cosEccentricAnomaly);
        const scalar meanAnomaly = eccentricAnomaly - initSetOfElements.eccentricity * sinEccentricAnomaly;

        return {initSetOfElements.inclination,
                initSetOfElements.longitudeOfAscendingNode,
                initSetOfElements.largeSemiAxis,
                initSetOfElements.eccentricity,
                initSetOfElements.pericenterArgument,
                meanAnomaly};
    }

    template<>
    inline KeplerMean convert<KeplerMean, KeplerMean>(const KeplerMean &initSetOfElements,
                                                      const scalar gravParam)
    {
        return initSetOfElements;
    }

    template<>
    inline KeplerTrue convert<KeplerTrue, KeplerTrue>(const KeplerTrue &initSetOfElements,
                                                      const scalar gravParam)
    {
        return initSetOfElements;
    }

////////////////////////////////////CONVERSION FROM KEPLERIAN TO CARTESIAN//////////////////////////////////////////////
    template<>
    inline PosVel convert<PosVel, KeplerTrue>(const KeplerTrue &initSetOfElements,
                                              const scalar gravParam)
    {
        // Calculations of trigonometric functions of Kepler's elements
        const scalar cosTrueAnomaly           = cos(initSetOfElements.trueAnomaly);
        const scalar sinTrueAnomaly           = sin(initSetOfElements.trueAnomaly);
        const scalar cosLongitOfAscendingNode = cos(initSetOfElements.longitudeOfAscendingNode);
        const scalar sinLongitOfAscendingNode = sin(initSetOfElements.longitudeOfAscendingNode);
        const scalar cosPericenterArg         = cos(initSetOfElements.pericenterArgument);
        const scalar sinPericenterArg         = sin(initSetOfElements.pericenterArgument);
        const scalar cosInclination           = cos(initSetOfElements.inclination);
        const scalar sinInclination           = sin(initSetOfElements.inclination);

        const scalar orbParam = initSetOfElements.largeSemiAxis * (1. - initSetOfElements.eccentricity *
                                                                        initSetOfElements.eccentricity);
        // Some auxiliary multipliers
        const scalar multiplier1 = orbParam / (1. + initSetOfElements.eccentricity * cosTrueAnomaly);
        const scalar multiplier2 = sqrt(gravParam / orbParam);

        const vector3d positionTmp = {multiplier1 * cosTrueAnomaly,
                                   multiplier1 * sinTrueAnomaly,
                                   0.};
        const vector3d velocityTmp = {(-1.) * multiplier2 * sinTrueAnomaly,
                                      multiplier2 * (initSetOfElements.eccentricity + cosTrueAnomaly),
                                      0.};
        const matrix3d rotationMatrix {{cosLongitOfAscendingNode * cosPericenterArg - sinLongitOfAscendingNode * sinPericenterArg * cosInclination,
                                       (-1.) * cosLongitOfAscendingNode * sinPericenterArg - sinLongitOfAscendingNode * cosPericenterArg * cosInclination,
                                       sinLongitOfAscendingNode * sinInclination},
                                      {sinLongitOfAscendingNode * cosPericenterArg + cosLongitOfAscendingNode * sinPericenterArg * cosInclination,
                                       (-1.) * sinLongitOfAscendingNode * sinPericenterArg + cosLongitOfAscendingNode * cosPericenterArg * cosInclination,
                                       (-1.) * cosLongitOfAscendingNode * sinInclination},
                                      {sinPericenterArg * sinInclination,
                                       cosPericenterArg * sinInclination,
                                       cosInclination}};
        return {rotationMatrix * positionTmp, rotationMatrix * velocityTmp};
    }

    template<>
    inline PosVel convert<PosVel, KeplerMean>(const KeplerMean &initSetOfElements,
                                              const scalar gravParam)
    {
        const KeplerTrue KeplerTrueElements = convert<KeplerTrue>(initSetOfElements, gravParam);
        return convert<PosVel>(KeplerTrueElements, gravParam);
    }
}

#endif //PROJECT_BALLISTICS_ORBITCONVERTER_HPP
