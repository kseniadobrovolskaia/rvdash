#include "Error.h"




namespace rvdash {

void failWithError(const std::string &Msg) {
  throw std::logic_error(Msg);
}

} // namespace rvdash


