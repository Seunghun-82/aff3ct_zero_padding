/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR_inter_std.
 */
#ifndef DECODER_RSC_BCJR_INTER_STD_HPP_
#define DECODER_RSC_BCJR_INTER_STD_HPP_

#include <vector>

#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter.hpp"
#include "Tools/Math/max.h"

namespace aff3ct
{
namespace module
{
template<typename B = int, typename R = float, tools::proto_max_i<R> MAX = tools::max_i>
class Decoder_RSC_BCJR_inter_std : public Decoder_RSC_BCJR_inter<B, R>
{
  public:
    Decoder_RSC_BCJR_inter_std(const int& K,
                               const std::vector<std::vector<int>>& trellis,
                               const bool buffered_encoding = true);
    virtual ~Decoder_RSC_BCJR_inter_std() = default;

    virtual Decoder_RSC_BCJR_inter_std<B, R, MAX>* clone() const;

  protected:
    int _decode_siso_alt(const R* sys, const R* par, R* ext, const size_t frame_id);

    void compute_gamma(const R* sys, const R* par);
    void compute_alpha();
    void compute_beta();
    void compute_ext(const R* sys, R* ext);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Inter/Decoder_RSC_BCJR_inter_std.hxx"
#endif

#endif /* DECODER_RSC_BCJR_INTER_STD_HPP_ */
