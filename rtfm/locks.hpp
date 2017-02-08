
#pragma once

#include <cstdint>
#include "rtfm/arm_intrinsics.hpp"
#include "rtfm/barriers.hpp"


namespace rtfm
{
namespace srp
{

/**
 * @brief  The definition of a lock in the SRP version of RTFM++.
 *
 * @tparam Resource   The resource to lock.
 */
template <typename Resource>
class lock
{
private:
  /**
   * @brief  Alias of the type of the BASEPRI register.
   */
  using basepri_t = decltype( arm_intrinsics::get_BASEPRI() );

  /**
   * @brief  Holder of old BASEPRI while the lock is active.
   */
  const basepri_t _old_basepri;

public:
  /**
   * @brief  The constructor locks the resource by manipulating BASEPRI.
   */
  lock() : _old_basepri( arm_intrinsics::get_BASEPRI() )
    /* Save old BASEPRI before execution of constructor body as per
     * C++ Standard §12.6.2 */
  {
    /* Lock the resource. */
    arm_intrinsics::set_BASEPRI_MAX( 5 /* getSRPResourceCeiling<Resource>::value */ );

    /* Barriers to guarantee the instruction took hold before continuing. */
    core::barrier_entry();
  }

  /**
   * @brief  The destructor releases the resource by restoring BASEPRI.
   */
  ~lock()
  {
    /* Barriers to guarantee no reordering before continuing. */
    core::barrier_exit();

    /* Unlock the resource. */
    arm_intrinsics::set_BASEPRI(_old_basepri);
  }
};

} /* END namespace srp */
} /* END namespace rtfm */
