#ifndef PRECICE_NO_MPI

#include "testing/Testing.hpp"

#include <precice/SolverInterface.hpp>
#include "helpers.hpp"

BOOST_AUTO_TEST_SUITE(PreciceTests)
BOOST_AUTO_TEST_SUITE(Serial)
BOOST_AUTO_TEST_SUITE(MappingNearestProjection)
BOOST_AUTO_TEST_CASE(MappingNearestProjectionImplicitEdges)
{
  /**
 * @brief Tests the Nearest Projection Mapping between two participants with explicit definition of edges
 *
 */
  PRECICE_TEST("SolverOne"_on(1_rank), "SolverTwo"_on(1_rank));
  bool defineEdgesExplicitly = false;
  testMappingNearestProjection(defineEdgesExplicitly, context.config(), context);
}

BOOST_AUTO_TEST_SUITE_END() // PreciceTests
BOOST_AUTO_TEST_SUITE_END() // Serial
BOOST_AUTO_TEST_SUITE_END() // MappingNearestProjection

#endif // PRECICE_NO_MPI
