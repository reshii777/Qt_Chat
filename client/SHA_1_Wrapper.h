/**
\file SHA_1_Wrapper.h
\brief Calculate sha_1 hash of string
*/

#ifndef SHA_1_WRAPPER_H
#define SHA_1_WRAPPER_H

#include <string>

namespace sha_1 {
  /**
  Calculate hash sha_1
  \param[in] value Input
  \return Hash
  */
  std::string hash(const std::string& value);
}

#endif
